#include <iostream>
#include <string>
#include <vector>
#include "Tools.h"
#include "Province.h"

using namespace std;

int main(int argc, char* argv[])
{
    vector<string> arguments;
    for(int i = 0 ; i < argc ; i++)
    {
        if(i == 0)
            continue;
        string tmp(argv[i]);
        arguments.push_back(tmp);
    }

    Province province = Province(arguments);
    province.fillUnnamedPipes();
    province.forkCity();
    int result = province.findResult();
    
    province.writeResultInNamedPipe(result);

    return 0;
}