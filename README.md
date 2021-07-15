# ZOIDBOL

Because sometimes you find yourself needing a portable command line options parser, and feel like a Decapodian.

## Example

See tests/example.cpp.

```cpp
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
```

Single character options are defined for '-' classic Unix style options. Values can be specified like "-o value" or "-ovalue".

Multiple character options are defined for "--" GNU style long options. Values can be specified like "--option value" or "--option=value".

The standard representation of a value is a string. Some helpers provided. If you want fancier: supply a callback. Look at the default_help_option for an example.

Problems throw CommandLineError which is derived from std::runtime_error.

StdCommandLineOption and StdCommandLineParser are templates that use std::string and std::vector\<std::string\>.

## Building

Zoidbol is a header only interface library. CMake support is provided. The install, package, and test targets do what you'd think.

## Debugging

Define ZOIDBOL_ENABLE_DEBUG to enable debug messages.

By default these are sent to std::cout and terminated with std::endl. To customize: define ZOIDBOL_DEBUG(str) before including any zoidbol header.
