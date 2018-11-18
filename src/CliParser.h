/*

MIT License

Copyright (c) 2018 Chris McArthur, prince.chrismc(at)gmail(dot)com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*
 *
 * Parses command line. Every element of the command line must be separated by space.
 * Additional interpretation is as follows:
 * defined on CL as "/a=b" = pair
 * defined on CL as "/a"   = switch
 * defined on CL as "a"    = non-interpreted
 * The '/' can also be a '-'
 * Arguments are case insensitive.
 *
 */

#pragma once

#include <string>
#include <vector>

class CommandLineParser
{
public:
#ifdef __cpp_fold_expressions
   template<typename ...Args>
   CommandLineParser( Args&&... args );
#endif

   CommandLineParser( int argc, char** argv );

   bool        DoesSwitchExists( const std::string& name ) const noexcept;
   std::string GetPairValue( std::string name ) const;
   std::string GetNonInterpted( size_t index ) const; // Ignores switches and pairs

   using ArgIterator = std::vector<std::string>::const_iterator;
   ArgIterator cbegin() const noexcept { return m_vecArgs.cbegin(); }
   ArgIterator cend() const noexcept { return m_vecArgs.cend(); }
   ArgIterator find( const std::string& name ) const noexcept;

   // Returns index to list elem or -1 for not found
   template<typename Enum>
   static auto doesMatch( const ArgIterator& arg_itor, std::initializer_list<std::string> list )-> Enum
   {
      for( size_t i = 0; i < list.size(); ++i )
      {
         if( toLower( *( list.begin() + i ) ) == toLower( *arg_itor ) )
            return Enum( i );
      }

      return Enum( -1 ); // Not Found
   }

private:
   CommandLineParser& operator<<(const std::string& arg );
   static std::string toLower( const std::string& str );

   std::vector<std::string>  m_vecArgs;   // all args written on command line
   std::string               m_sCommand;  // the name of the program invoked
};
