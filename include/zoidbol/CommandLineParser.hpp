// vim: set filetype=cpp tabstop=4 shiftwidth=4 expandtab :
#ifndef ZOIDBOL_COMMANDLINEPARSER__HPP
#define ZOIDBOL_COMMANDLINEPARSER__HPP
/*-
 * Copyright (c) 2021-current, Terry Mathew Poulin <BigBoss1964@gmail.com>
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <zoidbol/CommandLineOption.hpp>
#include <zoidbol/CommandLineError.hpp>
#include <zoidbol/DebugStream.hpp>

#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#include <vector>

#include <iostream>

namespace zoidbol
{

    /** Main parser interface.
     * 
     * Create the parser, add the options, and call the parse() with the
     * input arguments.
     */
    template <class OptionType>
    class CommandLineParser
    {
      public:
        typedef OptionType option_type;
        typedef OptionType* option_ptr;
        typedef OptionType& option_ref;
        typedef typename option_type::string_type string_type;
        typedef typename option_type::stringlist_type stringlist_type;
        typedef std::vector<option_ptr> option_list;

        /** Create the parser with default configuration.
         */
        CommandLineParser()
        {
        }

        /** Add a command line option.
         * 
         * @param option the CommandLineOption.
         */
        CommandLineParser& add_option(const option_ptr option)
        {
            std::back_inserter(mOptions) = option;
            return *this;
        }

        template <class InputIt>
        CommandLineParser& add_options(InputIt first, InputIt last)
        {
            std::copy(first, last, std::back_inserter(mOptions));
            return *this;
        }

        #if __cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) // C++14 required.
        /** Callback for standard help option.
         * 
         * Provides a standard callback for a --help like option that simply
         * calls usage() on this parser and then calls std::exit().
         * 
         * @param output passed to usage().
         * @param code passed to std::exit().
         */
        StdCommandLineOption::callback_type default_help_callback(std::ostream& output, int code)
        {
            return [this, &output, code](const string_type& unused) -> bool {
                (void)unused;
                this->usage(output);
                std::exit(code);
            };
        }
        #endif

        #if __cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) // C++14 required.
        /** Add a standard help option.
         * 
         * Defines an option for the flags "h", "help", and "?"  with the default_help_callback(std::cout, EXIT_SUCCESS).
         */
        option_type default_help_option()
        {
            return StdCommandLineOption({"h", "help", "?"}, "", "Display this help message.", StdCommandLineOption::NO_ARGUMENT, default_help_callback(std::cout, EXIT_SUCCESS));
        }
        #endif

        /** Set the program name.
         * 
         * @param name used for usage, etc.
         * @returns *this.
         */
        CommandLineParser& set_name(const string_type& name)
        {
            mProgram = name;
            return *this;
        }

        /** @returns the program name.
         * 
         * This is set from automatically from parse() or directly by
         * set_name().
         */
        const string_type& name() const
        {
            return mProgram;
        }

        /** Parse a main() style argument list.
         * 
         * This assumes that argv[0] is the name of the program.
         * 
         * @param argc same as main().
         * @param argv same as main().
         */
        void parse(int argc, const char* const argv[])
        {
            if (argc == 0)
                return;

            stringlist_type args;

            for (int i = 1; i < argc; ++i) {
                std::back_inserter(args) = argv[i];
            }

            parse(argv[0], args);
            ZOIDBOL_DEBUG("parse(argc, argv) return");
        }

        /** Parse command line options.
         * 
         * @param name the program name, passed to set_name().
         * @param args the argument list.
         */
        void parse(const string_type& name, const stringlist_type& args)
        {
            set_name(name);
            parse(args);
            ZOIDBOL_DEBUG("parse(name, args) return");
        }

        /** Parse a list of options.
         * 
         * This assumes the list is purely arguments.
         */
        void parse(const stringlist_type& args)
        {
            bool looking_for_options = true;

            for (typename stringlist_type::const_iterator arg = args.begin(); arg != args.end(); ++arg) {
                ZOIDBOL_DEBUG("args++; " << *arg << " looking_for_options: " << looking_for_options);

                if (arg->empty())
                    break;
                if (looking_for_options) {
                    if (arg->find("--") == 0) {
                        /* -- means stop parsing args. */
                        if (arg->size() == 2) {
                            ZOIDBOL_DEBUG("found --");
                            break;
                        } else {
                            ZOIDBOL_DEBUG("call parse_long_option() from arg: " << *arg);
                            arg = parse_long_option(arg, args.end());
                            continue;
                        }
                    } else if (arg[0][0] == '-') {
                            ZOIDBOL_DEBUG("call parse_short_option() from arg: " << *arg);
                        arg = parse_short_options(arg, args.end());
                        continue;
                    }

                    /* Unknown / non option. */
                    ZOIDBOL_DEBUG("parse(): start parsing at " << *arg);
                    looking_for_options = false;
                    ZOIDBOL_DEBUG("parse(): INNER REMAINING ARG: " << *arg);
                    mArguments.push_back(*arg);
                } else {
                    // TBD: remaining args.
                    ZOIDBOL_DEBUG("parse(): OUTER REMAINING ARG: " << *arg);
                    mArguments.push_back(*arg);
                }
            }
            ZOIDBOL_DEBUG("parse(args) return");
        }

        /** @returns arguments remaining after parsing the options.
         */
        const stringlist_type& arguments() const
        {
            return mArguments;
        }

        /** Access to the attached options.
         */
        const option_list& options() const
        {
            return mOptions;
        }

        /** Write standard usage message.
         * 
         * @param out the output stream to write to.
         */
        void usage(std::ostream& out)
        {
            using std::endl;

            out << "usage: " << mProgram;
            if (!mOptions.empty())
                out << " [options]";
            out << endl;

            for (typename option_list::const_iterator optit = mOptions.begin(); optit != mOptions.end(); ++optit) {
                option_ptr opt = *optit;
                for (size_t i=0; i < opt->flags().size(); ++i) {
                    const string_type& flag = opt->flags().at(i);
                    if (i > 0)
                        out << ", ";
                    if (flag.size() == 1)
                        out << '-' << flag;
                    else
                        out << "--" << flag;
                }
                out << '\t' << opt->help() << std::endl;
            }
        }

      private:
        option_list mOptions;
        string_type mProgram;
        stringlist_type mArguments;

        typename stringlist_type::const_iterator parse_short_options(typename stringlist_type::const_iterator arg, typename stringlist_type::const_iterator last)
        {
            ZOIDBOL_DEBUG("parse_short_options(): arg: " << *arg << " arg->size(): " << arg->size());

            size_t bounds = arg->size();
            const char* a = arg->c_str();
            string_type value;
            const char* missing_arg = "parse_short_option(): arg required but not given!";

            for (size_t i = 1; i < bounds; ++i) {
                ZOIDBOL_DEBUG("testing for " << a[i] << std::endl);
                for (typename option_list::iterator optit = mOptions.begin(); optit != mOptions.end(); ++optit) {
                    option_ptr opt = *optit;
                    const stringlist_type& flags = opt->flags();
                    bool found = false;
                    for (typename stringlist_type::const_iterator flag = opt->flags().begin(); flag != opt->flags().end(); ++flag) {
                        /* long flags. */
                        if (flag->size() > 1)
                            continue;

                        ZOIDBOL_DEBUG(flag->at(0) << " == a[" << i << "]: " << a[i]);
                        if (flag->at(0) == a[i]) {
                            ZOIDBOL_DEBUG("MATCH: " << a[i]);
                            switch (opt->mode()) {
                                case option_type::NO_ARGUMENT:
                                    opt->callback("true");
                                    break;
                                case option_type::ARGUMENT_REQUIRED:
                                    /* Can be like "-[opts]o arg" or "-[opts]oarg" */
                                    if ((bounds - i) > 1) {
                                        value = arg->substr(i + 1);
                                        if (value.empty()) {
                                            throw CommandLineError(missing_arg);
                                        }
                                        i += value.size();
                                    } else {
                                        arg = std::next(arg);
                                        if (arg == last || (!arg->empty() && arg->at(0) == '-')) {
                                            throw CommandLineError(missing_arg);
                                        }
                                        value = *arg;
                                    }
                                    opt->callback(value);
                                    break;
                                case option_type::ARGUMENT_OPTIONAL:
                                    ZOIDBOL_DEBUG("XXX: TODO: optional args for short options ...");
                                    break;
                                default:
                                    throw std::logic_error("zoidbol::CommandLineParser::parse_short_options(): invalid opt->mode()");
                            }
                            found = true;
                            break;
                        }
                    }
                    ZOIDBOL_DEBUG("++opt; found: " << found);
                    if (found)
                        break;
                }
            }

            ZOIDBOL_DEBUG("parse_short_options() return");
            return arg;
        }

        typename stringlist_type::const_iterator parse_long_option(typename stringlist_type::const_iterator arg, typename stringlist_type::const_iterator last)
        {
            ZOIDBOL_DEBUG("parse_long_option(" << *arg << ")");
            for (typename option_list::iterator optit = mOptions.begin(); optit != mOptions.end(); ++optit) {
                option_ptr opt = *optit;
                const stringlist_type& flags = opt->flags();
                for (typename stringlist_type::const_iterator flag = opt->flags().begin(); flag != opt->flags().end(); ++flag) {
                    ZOIDBOL_DEBUG("test flag " << *flag);
                    // short flags.
                    if (flag->size() == 1)
                        continue;

                    const char *a = arg->c_str();
                    a += 2; // skip --

                    if (strcmp(flag->c_str(), a) == 0 || strncmp(flag->c_str(), a, std::min(arg->size(), flag->size())) == 0) {
                        string_type value;

                        if (opt->mode() == option_type::NO_ARGUMENT) {
                            // na na na
                            value = "true";
                        } else {
                            typename string_type::size_type equals = arg->find('=');
                            if (equals != string_type::npos) {
                                value = arg->substr(equals + 1);
                                if (value.empty() && opt->mode() == option_type::ARGUMENT_REQUIRED)
                                    throw CommandLineError("parse_long_option(): arg required but not given!");
                            } else {
                                typename stringlist_type::const_iterator next_arg = std::next(arg);
                                if (next_arg == last) {
                                    if (opt->mode() == option_type::ARGUMENT_REQUIRED)
                                        throw CommandLineError("parse_long_option(): arg required but not given!");
                                }
                                if (opt->mode() == option_type::ARGUMENT_OPTIONAL) {
                                    if ((*next_arg)[0] == '-') {
                                        ZOIDBOL_DEBUG("XXX: TODO: optional args should only consue next_arg if it is not a registered option...");
                                    }
                                }
                                value = *next_arg;
                                arg = std::next(arg);
                            }
                            ZOIDBOL_DEBUG("value = *next_arg = " << value);
                        }
                        /* TBD: return value, args. */
                        ZOIDBOL_DEBUG("opt->callback(" << value << ")");
                        opt->callback(value);
                        ZOIDBOL_DEBUG("opt->callback(" << value << ") -> opt->to_string(): " << opt->to_string());
                        break;
                    }
                }
            }

            ZOIDBOL_DEBUG("parse_long_option() return");
            return arg;
        }
    };

    /** Typedef using std::string and std::vector.
     */
    typedef CommandLineParser<StdCommandLineOption> StdCommandLineParser;

} // namespace zoidbol

#endif // ZOIDBOL_COMMANDLINEPARSER__HPP
