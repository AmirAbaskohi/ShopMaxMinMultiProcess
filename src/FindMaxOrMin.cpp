#include "FindMaxOrMin.h"

using namespace std;

FindMaxOrMin :: FindMaxOrMin (string command, string assetsDirName)
{
    vector<string> splittedCommand;
    splittedCommand = Tools :: splitString(command, SPACE);
    isMax = ( splittedCommand[0] == MAX ) ? true : false;
    productId = stoi(splittedCommand[1]);
    startingDate = splittedCommand[2];
    endingDate = splittedCommand[3];
    assetsDir = "../" + assetsDirName;
    this -> setFiles();
    this -> createUnnamedPipes();
    this -> createNamedPipes();
}

FindMaxOrMin :: ~FindMaxOrMin ()
{

}

void FindMaxOrMin :: setFiles()
{
    DIR *dir;
    struct dirent *ent;
    int isProvince = 0;
    if ((dir = opendir(assetsDir.c_str())) != NULL)
    {
        while((ent = readdir(dir)) != NULL)
        {
            if(isProvince == 2)
                provinceDirs.push_back(ent->d_name);
            else
                isProvince++;
        }
    }
    else
        cout << "Error In  Opening Directory -> " + assetsDir << endl; 
}

void FindMaxOrMin :: createUnnamedPipes()
{
    for(int i = 0 ; i < provinceDirs.size() ; i++)
    {
        int fd[2];
        if(pipe(fd) < 0)
            cout << "Pipe exception in FindMaxOrMin" << endl;
        else
        {
            vector<int> fds;
            fds.push_back(fd[0]);
            fds.push_back(fd[1]);
            unnamedPipes.push_back(fds);
        }
        
    }
}

void FindMaxOrMin :: createNamedPipes()
{
    for(int i = 0 ; i < provinceDirs.size() ; i++)
    {
        string provinceFifoPath = Tools :: getFifoPath(provinceDirs[i]);
        mkfifo(provinceFifoPath.c_str(), 0666);
    }
}

void FindMaxOrMin :: forkProvince()
{
    for(int i = 0 ; i < provinceDirs.size() ; i++)
    {
        pid_t pid = fork();
        if(pid < 0)
            cout << "Error in forking province" << endl;
        else if(pid == 0)
            runProvince(i);
        else
        {
            string provinceFifoPath = Tools :: getFifoPath(provinceDirs[i]);
            int fd = open(provinceFifoPath.c_str(), O_RDONLY);
            wait(NULL);
            char buff[MAX_BUFF];
            int n = read(fd, buff, MAX_BUFF);
            buff[n] = NULL_CHAR;
            provinceResults.push_back(atoi(buff));
            close(fd);
        }
    }
}

void FindMaxOrMin :: runProvince(int index)
{
    char buff[MAX_BUFF];
    int n = read(unnamedPipes[index][READ], buff, MAX_BUFF);
    close(unnamedPipes[index][READ]);
    buff[n] = NULL_CHAR;

    string buffString(buff);
    vector<string> buffWords = Tools::splitString(buffString, DASH); 

    char* argv[8];
    argv[0] = (char*) PROVINCE_EXEC_PATH;
    argv[1] = (char*) buffWords[0].c_str();
    argv[2] = (char*) buffWords[1].c_str();
    argv[3] = (char*) buffWords[2].c_str();
    argv[4] = (char*) buffWords[3].c_str();
    argv[5] = (char*) buffWords[4].c_str();
    argv[6] = (char*) buffWords[5].c_str();
    argv[7] = NULL;

    execv(argv[0], argv);
}

void FindMaxOrMin :: fillUnnamedPipes()
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
        result += provinceDirs[i];
        write(unnamedPipes[i][WRITE], result.c_str(), result.size());
        close(unnamedPipes[i][WRITE]);
    }
}

int FindMaxOrMin :: findMax()
{
    int result = -2147483648;
    for(int i = 0 ; i < provinceResults.size() ; i++)
    {
        int value = provinceResults[i];

        if(value > result)
            result = value;
    }
    return result;
}

int FindMaxOrMin :: findMin()
{
    int result = 2147483647;
    for(int i = 0 ; i < provinceResults.size() ; i++)
    {
        int value = provinceResults[i];

        if(value < result)
            result = value;
    }
    return result;
}

int FindMaxOrMin :: findResult()
{
    if(provinceDirs.size() == 0)
        return -1;
    if(isMax)
        return findMax();
    return findMin();
}
