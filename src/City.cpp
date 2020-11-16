#include "City.h"

using namespace std;

City :: City (vector<string>arguments)
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

City :: ~City()
{

}

void City :: setFiles()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(assetsDir.c_str())) != NULL)
    {
        while((ent = readdir(dir)) != NULL)
        {
            size_t found;
            string fileName(ent->d_name);
            found = fileName.find(CSV_FORMAT);

            if(found != string::npos)
                shopFiles.push_back(fileName);
        }
    }
    else
        cout << "Error In  Opening Directory -> " + assetsDir << endl; 
}

void City :: createUnnamedPipes()
{
    for(int i = 0 ; i < shopFiles.size() ; i++)
    {
        int fd[2];
        if(pipe(fd) < 0)
            cout << "Pipe exception in City" << endl;
        else
        {
            vector<int> fds;
            fds.push_back(fd[0]);
            fds.push_back(fd[1]);
            unnamedPipes.push_back(fds);
        }
        
    }
}

void City :: createNamedPipes()
{
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < shopFiles.size() ; i++)
    {
        string shopName = Tools::splitString(shopFiles[i], DOT)[0];
        string fifoName = assetsSplitted[assetsSplitted.size()-2] + UNDERSCORE + assetsSplitted[assetsSplitted.size()-1] + UNDERSCORE + shopName;
        string provinceFifoPath = Tools :: getFifoPath(fifoName);
        mkfifo(provinceFifoPath.c_str(), 0666);
    }
}

void City :: forkShop()
{
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < shopFiles.size() ; i++)
    {
        pid_t pid = fork();
        if(pid < 0)
            cout << "Error in forking shop" << endl;
        else if(pid == 0)
            runShop(i);
        else
        {
            string shopName = Tools::splitString(shopFiles[i], DOT)[0];
            string fifoName = assetsSplitted[assetsSplitted.size()-2] + UNDERSCORE + assetsSplitted[assetsSplitted.size()-1] + UNDERSCORE + shopName;
            string provinceFifoPath = Tools :: getFifoPath(fifoName);
            int fd = open(provinceFifoPath.c_str(), O_RDONLY);
            wait(NULL);
            char buff[MAX_BUFF];
            int n = read(fd, buff, MAX_BUFF);
            buff[n] = NULL_CHAR;
            shopResults.push_back(atoi(buff));
            close(fd);
        }
    }
}

void City :: runShop(int index)
{
    char buff[MAX_BUFF];
    int n = read(unnamedPipes[index][READ], buff, MAX_BUFF);
    close(unnamedPipes[index][READ]);
    buff[n] = NULL_CHAR;

    string buffString(buff);
    vector<string> buffWords = Tools::splitString(buffString, DASH); 

    char* argv[8];
    argv[0] = (char*) SHOP_EXEC_PATH;
    argv[1] = (char*) buffWords[0].c_str();
    argv[2] = (char*) buffWords[1].c_str();
    argv[3] = (char*) buffWords[2].c_str();
    argv[4] = (char*) buffWords[3].c_str();
    argv[5] = (char*) buffWords[4].c_str();
    argv[6] = (char*) buffWords[5].c_str();
    argv[7] = NULL;

    execv(argv[0], argv);
}

void City :: fillUnnamedPipes()
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
        result += shopFiles[i];
        write(unnamedPipes[i][WRITE], result.c_str(), result.size());
        close(unnamedPipes[i][WRITE]);
    }
}

int City :: findMax()
{
    int result = -2147483648;
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < shopResults.size() ; i++)
    {
        int value = shopResults[i];

        if(value > result)
            result = value;
    }
    return result;
}

int City :: findMin()
{
    int result = 2147483647;
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    for(int i = 0 ; i < shopResults.size() ; i++)
    {
        int value = shopResults[i];

        if(value < result)
            result = value;
    }
    return result;
}

int City :: findResult()
{
    if(shopFiles.size() == 0)
        return -1;
    if(isMax)
        return findMax();
    return findMin();
} 

void City :: writeResultInNamedPipe(int result)
{
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    string pipeName = assetsSplitted[assetsSplitted.size()-2] + UNDERSCORE + assetsSplitted[assetsSplitted.size()-1];
    string pipePath = Tools::getFifoPath(pipeName);
    int fd = open(pipePath.c_str(), O_WRONLY);

    string resultString = to_string(result);
    write(fd, resultString.c_str(), resultString.size());

    close(fd);
}