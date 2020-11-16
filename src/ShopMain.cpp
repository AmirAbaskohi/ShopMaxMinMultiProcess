#include <iostream>
#include <string>
#include <vector>
#include "Tools.h"
#include "Shop.h"

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

    Shop shop = Shop(arguments);
    int result = shop.findResult();
    
    shop.writeResultInNamedPipe(result);

    return 0;
}