#include "Province.h"

using namespace std;

Province :: Province (vector<string>arguments)
{
    isMax = (arguments[0] == MAX) ? true : false;
    assetsDir = arguments[4] + SLASH + arguments[5];
    startingDate = arguments[2];
    endingDate = arguments[3];
    productId = stoi(arguments[1]); 
    this -> setFiles();
    this -> createUnnamedPipes();
    this -> createNamedPipes();
}

Province :: ~Province()
{

}

void Province :: setFiles()
{
    DIR *dir;
    struct dirent *ent;
    int isCity = 0;
    if ((dir = opendir(assetsDir.c_str())) != NULL)
    {
        while((ent = readdir(dir)) != NULL)
        {
            if(isCity == 2)
                cityDirs.push_back(ent->d_name);
            else
                isCity++;
        }
    }
    else
        cout << "Error In  Opening Directory -> " + assetsDir << endl; 
}

void Province :: createUnnamedPipes()
{
    for(int i = 0 ; i < cityDirs.size() ; i++)
    {
        int fd[2];
        if(pipe(fd) < 0)
            cout << "Pipe exception in Province" << endl;
        else
        {
            vector<int> fds;
            fds.push_back(fd[0]);
            fds.push_back(fd[1]);
            unnamedPipes.push_back(fds);
        }
        
    }
}

void Province :: createNamedPipes()
{
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < cityDirs.size() ; i++)
    {
        string fifoName = assetsSplitted[assetsSplitted.size()-1] + UNDERSCORE + cityDirs[i];
        string provinceFifoPath = Tools :: getFifoPath(fifoName);
        mkfifo(provinceFifoPath.c_str(), 0666);
    }
}

void Province :: forkCity()
{
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < cityDirs.size() ; i++)
    {
        pid_t pid = fork();
        if(pid < 0)
            cout << "Error in forking city" << endl;
        else if(pid == 0)
            runCity(i);
        else
        {
            string fifoName = assetsSplitted[assetsSplitted.size()-1] + UNDERSCORE + cityDirs[i];
            string provinceFifoPath = Tools :: getFifoPath(fifoName);
            int fd = open(provinceFifoPath.c_str(), O_RDONLY);
            wait(NULL);
            char buff[MAX_BUFF];
            int n = read(fd, buff, MAX_BUFF);
            buff[n] = NULL_CHAR;
            cityResults.push_back(atoi(buff));
            close(fd);
        }
    }
}

void Province :: runCity(int index)
{
    char buff[MAX_BUFF];
    int n = read(unnamedPipes[index][READ], buff, MAX_BUFF);
    close(unnamedPipes[index][READ]);
    buff[n] = NULL_CHAR;

    string buffString(buff);
    vector<string> buffWords = Tools::splitString(buffString, DASH); 

    char* argv[8];
    argv[0] = (char*) CITY_EXEC_PATH;
    argv[1] = (char*) buffWords[0].c_str();
    argv[2] = (char*) buffWords[1].c_str();
    argv[3] = (char*) buffWords[2].c_str();
    argv[4] = (char*) buffWords[3].c_str();
    argv[5] = (char*) buffWords[4].c_str();
    argv[6] = (char*) buffWords[5].c_str();
    argv[7] = NULL;

    execv(argv[0], argv);
}

void Province :: fillUnnamedPipes()
{
    string maxOrMin = isMax ? MAX : MIN;
    for(int i = 0 ; i < unnamedPipes.size() ; i++)
    {
        string result = maxOrMin;
        result += DASH;
        result += to_string(productId);
        result += DASH;
        result += startingDate;
        result += DASH;
        result += endingDate;
        result += DASH;
        result += assetsDir;
        result += DASH;
        result += cityDirs[i];
        write(unnamedPipes[i][WRITE], result.c_str(), result.size());
        close(unnamedPipes[i][WRITE]);
    }
}

int Province :: findMax()
{
    int result = -2147483648;
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < cityResults.size() ; i++)
    {
        int value = cityResults[i];

        if(value > result)
            result = value;
    }
    return result;
}

int Province :: findMin()
{
    int result = 2147483647;
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < cityResults.size() ; i++)
    {
        int value = cityResults[i];

        if(value < result)
            result = value;
    }
    return result;
}

int Province :: findResult()
{
    if(cityDirs.size() == 0)
        return -1;
    if(isMax)
        return findMax();
    return findMin();
} 

void Province :: writeResultInNamedPipe(int result)
{
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    string pipePath = Tools::getFifoPath(assetsSplitted[assetsSplitted.size()-1]);

    int fd = open(pipePath.c_str(), O_WRONLY);

    string resultString = to_string(result);
    write(fd, resultString.c_str(), resultString.size());

    close(fd);
}