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

#define CITY_EXEC_PATH "CityMain"

#define READ 0
#define WRITE 1

class Province
{
    public:
        Province(std::vector<std::string> arguments);
        ~Province();
        void setFiles();
        void createUnnamedPipes();
        void createNamedPipes();
        void forkCity();
        void runCity(int index);
        void fillUnnamedPipes();
        int findResult();
        int findMin();
        int findMax();
        void writeResultInNamedPipe(int result);

    private:
        std::string assetsDir;
        bool isMax;
        int productId;
        std::string startingDate;
        std::string endingDate;
        std::vector<std::string> cityDirs;
        std::vector<std::vector<int> > unnamedPipes;
        std::vector<int> cityResults;
};