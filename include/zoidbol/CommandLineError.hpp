// vim: set filetype=cpp tabstop=4 shiftwidth=4 expandtab :
#ifndef ZOIDBOL_COMMANDLINEERROR__HPP
#define ZOIDBOL_COMMANDLINEERROR__HPP
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

#include <exception>
#include <stdexcept>

namespace zoidbol
{
    /** Exception thrown by command line related errors.
     * 
     * Simple wrapper around std::runtime_error. Created so it is possible to
     * determine if an error was thrown specifically because of a command line
     * error. Such as a required argument missing.
     */
    class CommandLineError
        : public std::runtime_error
    {
      public:
        CommandLineError(const std::string& what_arg)
            : std::runtime_error(what_arg)
        {
        }

        CommandLineError(const char* what_arg)
            : std::runtime_error(what_arg)
        {
        }

        ~CommandLineError()
        {
        }
    };
} // namespace zoidbol

#endif // ZOIDBOL_COMMANDLINEERROR__HPP