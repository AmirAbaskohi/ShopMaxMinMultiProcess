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

#define READ 0
#define WRITE 1

class Shop
{
    public:
        Shop(std::vector<std::string> arguments);
        ~Shop();
        int findResult();
        int findMin();
        int findMax();
        void writeResultInNamedPipe(int result);
        void readCSVFile();

    private:
        std::string assetsDir;
        bool isMax;
        int productId;
        std::string startingDate;
        std::string endingDate;
        std::vector<int> fileProductIds;
        std::vector<std::string> fileDates;
        std::vector<int> filePrices;
};