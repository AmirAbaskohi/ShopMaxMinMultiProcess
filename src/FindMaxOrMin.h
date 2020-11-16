#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits>
#include "Tools.h"

#define MAX "MAX"
#define MIN "MIN"

#define PROVINCE_EXEC_PATH "ProvinceMain"

#define READ 0
#define WRITE 1

class FindMaxOrMin
{
    public:
        FindMaxOrMin(std::string command, std::string assetsDirName);
        ~FindMaxOrMin();
        void setFiles();
        void createUnnamedPipes();
        void createNamedPipes();
        void forkProvince();
        void runProvince(int index);
        void fillUnnamedPipes();
        int findResult();
        int findMin();
        int findMax();

    private:
        std::string assetsDir;
        bool isMax;
        int productId;
        std::string startingDate;
        std::string endingDate;
        std::vector<std::string> provinceDirs;
        std::vector<std::vector<int> > unnamedPipes;
        std::vector<int> provinceResults;
};