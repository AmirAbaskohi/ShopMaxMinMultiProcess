#include "Tools.h"
#include <iostream>

using namespace std;

vector<string> Tools :: readCommandsOfFile(string fileName)
{
    vector<string> commands;
    ifstream CommandFile(fileName);
    string command;

    while(getline(CommandFile, command))
    {
        commands.push_back(command);
        command = EMPTY_STRING;
    }

    CommandFile.close();

    return commands;
}

vector<string> Tools :: splitString(string str, string delimiter)
{
    size_t pos = 0;
    string token;
    vector<string> result;
    while ((pos = str.find(delimiter)) != string::npos)
    {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);

    return result;
}

string Tools :: getFifoPath(string name)
{
    string result = FIFO_FILE_PATH;
    result += "/";
    result += name;

    return result;
}

bool Tools :: isDateInPeriod(string date, string start, string end)
{
    int startYear, endYear, startMonth, endMonth, startDay, endDay;
    int year, month, day;
    vector<string> splittedDate;
    splittedDate = Tools :: splitString(date, SLASH);
    year = stoi(splittedDate[0]);
    month = stoi(splittedDate[1]);
    day = stoi(splittedDate[2]);
    splittedDate.clear();

    splittedDate = Tools :: splitString(start, SLASH);
    startYear = stoi(splittedDate[0]);
    startMonth = stoi(splittedDate[1]);
    startDay = stoi(splittedDate[2]);
    splittedDate.clear();

    splittedDate = Tools :: splitString(end, SLASH);
    endYear = stoi(splittedDate[0]);
    endMonth = stoi(splittedDate[1]);
    endDay = stoi(splittedDate[2]);
    splittedDate.clear();

    if(year < startYear || year > endYear)
        return false;
    if((year == startYear && month < startMonth) || (year == endYear && month > endMonth))
        return false;
    if((year == startYear && month == startMonth && day < startDay) || (year == endYear && month == endMonth && day > endDay))
        return false;
    return true;
}