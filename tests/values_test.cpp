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

    zoidbol::StdCommandLineOption boolean_flag({"b", "boolean"}, "false", "Set a boolean flag.", zoidbol::StdCommandLineOption::NO_ARGUMENT);
    zoidbol::StdCommandLineOption string_flag({"s", "string"}, "", "Set a flag to value.", zoidbol::StdCommandLineOption::ARGUMENT_REQUIRED);
    zoidbol::StdCommandLineOption optional_flag({"o", "optional"}, "", "Value is option", zoidbol::StdCommandLineOption::ARGUMENT_OPTIONAL);
    zoidbol::StdCommandLineOption help = parser.default_help_option();

    parser
        .add_option(&boolean_flag)
        .add_option(&string_flag)
        .add_option(&optional_flag)
        .add_option(&help)
        ;

    try {
        cout << "PARSE START" << endl;
        parser.parse(argc, argv);
        cout << "PARSE END" << endl;
    } catch (zoidbol::CommandLineError& ex) {
        std::clog << "CommandLineError: " << ex.what() << endl;
    }

    cout
    << "boolean_flag.to_bool(): " << (boolean_flag.to_bool() ? "true" : "false") << endl
    << "string_flag.to_string(): \"" << string_flag.to_string() << "\"" << endl
    << "optional_flag.to_string(): \"" << optional_flag.to_string() << "\"" << endl
    << endl;

    if (!boolean_flag.to_bool() && string_flag.to_string().empty())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
