// vim: set filetype=cpp tabstop=4 shiftwidth=4 expandtab :

#include <zoidbol/CommandLineError.hpp>
#include <zoidbol/CommandLineOption.hpp>
#include <zoidbol/CommandLineParser.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    zoidbol::StdCommandLineParser parser;

    // -b or --bolean
    zoidbol::StdCommandLineOption boolean_flag({"b", "boolean"}, "false", "Set a boolean flag.", zoidbol::StdCommandLineOption::NO_ARGUMENT);
    // -s value or --string value
    zoidbol::StdCommandLineOption string_flag({"s", "string"}, "", "Set a flag to value.", zoidbol::StdCommandLineOption::ARGUMENT_REQUIRED);
    // -h or --help do the most common usage then exit.
    zoidbol::StdCommandLineOption help = parser.default_help_option();

    parser
        .add_option(&boolean_flag)
        .add_option(&string_flag)
        .add_option(&help)
        ;

    try {
        parser.parse(argc, argv);
    } catch (zoidbol::CommandLineError& ex) {
        std::clog << "CommandLineError: " << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
