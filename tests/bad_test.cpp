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

    zoidbol::StdCommandLineOption flag({"o", "option-required"}, "", "A flag.", zoidbol::StdCommandLineOption::ARGUMENT_REQUIRED);
    zoidbol::StdCommandLineOption help = parser.default_help_option();

    parser.add_option(&flag).add_option(&help);

    try {
        parser.parse(argc, argv);
    } catch (CommandLineError& ex) {
        cout << "parser.parse(): CommandLineError: " << ex.what() << endl;
        cout << "return EXIT_SUCCESS" << endl;
        return EXIT_SUCCESS;
    }

    cout << "parser.parse() did not throw CommandLineError" << endl;
    cout << "return EXIT_FAILURE" << endl;
    return EXIT_FAILURE;
}
