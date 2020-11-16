#include <iostream>
#include <string>
#include <vector>
#include "FindMaxOrMin.h"

using namespace std;

int main(int argc, char* argv[])
{
    string commandsFileName(argv[1]);
    string assetsDirectoryName(argv[2]);
    vector<string> commands;

    commands = Tools :: readCommandsOfFile("../"+commandsFileName);

    for(int i = 0 ; i < commands.size() ; i++)
    {
        FindMaxOrMin findMaxOrMin = FindMaxOrMin(commands[i], assetsDirectoryName);
        findMaxOrMin.fillUnnamedPipes();
        findMaxOrMin.forkProvince();
        int result = findMaxOrMin.findResult();

        cout << result << endl;
    }

    return 0;
}