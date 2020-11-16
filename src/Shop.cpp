#include "Shop.h"

using namespace std;

Shop :: Shop (vector<string>arguments)
{
    isMax = (arguments[0] == MAX) ? true : false;
    assetsDir = arguments[4] + SLASH + arguments[5];
    startingDate = arguments[2];
    endingDate = arguments[3];
    productId = stoi(arguments[1]); 
    this -> readCSVFile();
}

Shop :: ~Shop()
{

}

void Shop :: readCSVFile()
{
    string line;
    vector<string> data;
    ifstream shopCSVFile(assetsDir);
    getline(shopCSVFile, line);
    while(getline(shopCSVFile, line))
    {
        data = Tools :: splitString(line, COMMA);
        fileDates.push_back(data[0]);
        fileProductIds.push_back(stoi(data[1]));
        filePrices.push_back(stoi(data[2]));
    }
    shopCSVFile.close();
}

int Shop :: findMax()
{
    bool hasValidData = false;
    int result = -2147483648;
    int numberOfDatasInFile = filePrices.size();
    for(int i = 0 ; i < numberOfDatasInFile ; i++)
    {
        if((fileProductIds[i] == productId) &&
         (Tools :: isDateInPeriod(fileDates[i], startingDate, endingDate)))
         {
             hasValidData = true;
             if(result < filePrices[i])
                result = filePrices[i];
         }
    }
    if(hasValidData)
        return result;
    return -1;
}

int Shop :: findMin()
{
    bool hasValidData = false;
    int result = 2147483647;
    int numberOfDatasInFile = filePrices.size();
    for(int i = 0 ; i < numberOfDatasInFile ; i++)
    {
        if((fileProductIds[i] == productId) &&
         (Tools :: isDateInPeriod(fileDates[i], startingDate, endingDate)))
         {
             hasValidData = true;
             if(result > filePrices[i])
                result = filePrices[i];
         }
    }
    if(hasValidData)
        return result;
    return -1;
}

int Shop :: findResult()
{
    if(filePrices.size() == 0)
        return -1;
    if(isMax)
        return findMax();
    return findMin();
} 

void Shop :: writeResultInNamedPipe(int result)
{
    vector<string> assetsSplitted = Tools :: splitString(assetsDir, SLASH);
    string shopName = Tools::splitString(assetsSplitted[assetsSplitted.size()-1], DOT)[0];
    string pipeName = assetsSplitted[assetsSplitted.size()-3] + UNDERSCORE + assetsSplitted[assetsSplitted.size()-2] + UNDERSCORE + shopName;
    string pipePath = Tools::getFifoPath(pipeName);
    int fd = open(pipePath.c_str(), O_WRONLY);
    string resultString = to_string(result);
    write(fd, resultString.c_str(), resultString.size());

    close(fd);
}