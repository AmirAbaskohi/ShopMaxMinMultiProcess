#include <iostream>
#include <string>
#include <vector>
#include "Tools.h"
#include "City.h"

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

    City city = City(arguments);
    city.fillUnnamedPipes();
    city.forkShop();
    int result = city.findResult();
    
    city.writeResultInNamedPipe(result);

    return 0;
}