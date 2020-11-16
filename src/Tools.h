#ifndef TOOLS
#define TOOLS

#include <string>
#include <vector>
#include <fstream>

#define EMPTY_STRING ""
#define FIFO_FILE_PATH "../fifo"

#define CSV_FORMAT ".csv"

#define SLASH "/"
#define SPACE " "
#define DASH "-"
#define UNDERSCORE "_"
#define DOT "."
#define COMMA ","

#define POS_INFINITY 2147483647
#define NEG_INFINITY -2147483648

#define MAIN_PIPE_NAME "MAIN_PIPE"

#define MAX_BUFF 1024
#define NULL_CHAR 0

class Tools
{
    public:
        static std::vector<std::string> readCommandsOfFile(std::string fileName);
        static std::vector<std::string> splitString(std::string str, std::string delimiter);
        static std::string getFifoPath(std::string name);
        static bool isDateInPeriod(std::string date, std::string start, std::string end);
};

#endif