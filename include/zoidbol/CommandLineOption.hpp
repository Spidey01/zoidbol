// vim: set filetype=cpp tabstop=4 shiftwidth=4 expandtab :
#ifndef ZOIDBOL_COMMANDLINEOPTION__HPP
#define ZOIDBOL_COMMANDLINEOPTION__HPP
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

#include <cstring>
#include <functional>
#include <string>
#include <vector>

#include <iostream>

#include <zoidbol/DebugStream.hpp>

// zoidbol::FOO vs zoidbol::classname::FOO.
// #include <zoidbol/ArgumentMode.hpp>

namespace zoidbol
{
    /** Template class for command line options.
     */
    template <class StringType, class ListType>
    class CommandLineOption
    {
      public:
        typedef StringType string_type;
        typedef ListType stringlist_type;
        typedef std::function<bool(const string_type&)> callback_type;

        enum ArgumentMode {
            NO_ARGUMENT,       /**< --option or bust. */
            ARGUMENT_REQUIRED, /**< --option VALUE or bust. */
            ARGUMENT_OPTIONAL, /**< --option and --option VALUE are okay. */
        };

        /** Create a command line option.
         * 
         * @param flags the flags to test for this option. Single character
         * flags will accept '-' as a prefix in the classic unix style.
         * Multiple character flags will accept a '--' prefix, GNU style.
         * 
         * @param defaultValue the default value for to_string().
         * @param argMode whether a value argument is required, optional, or banned.
         * @param help the help message.
         * @param callback function to call when found. Return true if option parsing should continue.
         */
        CommandLineOption(const stringlist_type& flags, const string_type& defaultValue, const string_type& help, ArgumentMode argMode, callback_type callback)
            : mFlags(flags)
            , mValue(defaultValue)
            , mHelp(help)
            , mMode(argMode)
            , mCallback(callback)
        {
        }

        /** Create a command line option with the default callback.
         * 
         * @param flags the flags to test for this option. Single character
         * flags will accept '-' as a prefix in the classic unix style.
         * Multiple character flags will accept a '--' prefix, GNU style.
         * 
         * @param defaultValue the default value for to_string().
         * @param argMode whether a value argument is required, optional, or banned.
         * @param help the help message.
         */
        CommandLineOption(const stringlist_type& flags, const string_type& defaultValue, const string_type& help, ArgumentMode argMode)
            : mFlags(flags)
            , mValue(defaultValue)
            , mHelp(help)
            , mMode(argMode)
            , mCallback()
        {
        }

        /**  Simple boolean options.
         * 
         * @param flags the flags to test for this option. Single character
         * flags will accept '-' as a prefix in the classic unix style.
         * Multiple character flags will accept a '--' prefix, GNU style.
         * 
         * @param defaultValue the default value for to_string().
         * @param help the help message.
         */
        CommandLineOption(const stringlist_type& flags, bool defaultValue, const string_type& help)
            : mFlags(flags)
            , mValue(defaultValue ? "true" : "false")
            , mHelp(help)
            , mMode(NO_ARGUMENT)
            , mCallback()
        {
        }

        /** Simple options that take a value.
         * 
         * @param flags the flags to test for this option. Single character
         * flags will accept '-' as a prefix in the classic unix style.
         * Multiple character flags will accept a '--' prefix, GNU style.
         * 
         * @param defaultValue the default value for to_string().
         * @param help the help message.
         */
        CommandLineOption(const stringlist_type& flags, const string_type& defaultValue, const string_type& help)
            : mFlags(flags)
            , mValue(defaultValue)
            , mHelp(help)
            , mMode(ARGUMENT_REQUIRED)
            , mCallback()
        {
        }

        /** @returns the list of flags for this option.
         */
        const stringlist_type& flags() const
        {
            return mFlags;
        }

        /** @returns our ArgumentMode.
         */
        ArgumentMode mode() const
        {
            return mMode;
        }

        /** @returns the help() message.
         */
        const string_type& help() const
        {
            return mHelp;
        }

        /** Execute the callback if set.
         * 
         * @param arg ...
         * @returns callback(...) if set, else true.
         */
        bool callback(const string_type& arg)
        {
            ZOIDBOL_DEBUG("callback(\"" << arg << "\")");
            bool ok = true;
            if (mCallback) {
                ok = mCallback(arg);
            }
            mValue = arg;
            return ok;
        }

        /** @returns value as a boolean.
         * 
         * All values other than "true" are considered false.
         */
        bool to_bool() const
        {
            if (mValue.empty())
                return false;
            return std::strcmp(mValue.c_str(), "true") == 0;
        }

        /** @returns value as a int.
         */
        int to_int() const
        {
            return std::stoi(mValue);
        }

        /** @returns value as a float.
         */
        float to_float() const
        {
            return std::stof(mValue);
        }

        /** @returns value as a string.
         */
        string_type to_string() const
        {
            return mValue;
        }

      private:
        stringlist_type mFlags;
        string_type mHelp;
        string_type mValue;
        ArgumentMode mMode;
        callback_type mCallback;
    };

    /** Typedef using std::string and std::vector.
     */
    typedef CommandLineOption<std::string, std::vector<std::string>> StdCommandLineOption;

} // namespace zoidbol

#endif // ZOIDBOL_COMMANDLINEOPTION__HPP
