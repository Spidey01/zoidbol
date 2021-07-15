// vim: set filetype=cpp tabstop=4 shiftwidth=4 expandtab :

#include <zoidbol/CommandLineOption.hpp>
#include <zoidbol/CommandLineParser.hpp>


#include <iostream>

using std::cout;
using std::endl;

using namespace zoidbol;

int main(int argc, char* argv[])
{
    cout << "argc: " << argc << endl;

    for (int i = 0; i < argc; ++i) {
        cout << "argv[" << i << "]: " << '"' << argv[i] << '"' << endl;
    }

    zoidbol::StdCommandLineParser parser;

    zoidbol::StdCommandLineOption a_flag({"a"}, "", "A flag.", zoidbol::StdCommandLineOption::NO_ARGUMENT);
    zoidbol::StdCommandLineOption b_flag({"b"}, "", "B flag", zoidbol::StdCommandLineOption::NO_ARGUMENT);
    zoidbol::StdCommandLineOption help({"h", "help", "?"}, "", "Display this help message.", zoidbol::StdCommandLineOption::NO_ARGUMENT, parser.default_help_callback(std::cout, EXIT_SUCCESS));

    parser.add_option(&a_flag).add_option(&b_flag).add_option(&help);

    parser.parse(argc, argv);

    return 0;
}
