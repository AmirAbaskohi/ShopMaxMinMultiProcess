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

#define SHOP_EXEC_PATH "ShopMain"

#define READ 0
#define WRITE 1

class City
{
    public:
        City(std::vector<std::string> arguments);
        ~City();
        void setFiles();
        void createUnnamedPipes();
        void createNamedPipes();
        void forkShop();
        void runShop(int index);
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
        std::vector<std::string> shopFiles;
        std::vector<std::vector<int> > unnamedPipes;
        std::vector<int> shopResults;
};