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

#include "CliParser.h"
#include <algorithm>
#include <type_traits>
#include <cctype>

#ifdef __cpp_lib_execution
#include <execution>
#endif

#ifdef __cpp_fold_expressions
template <typename ... Args>
CommandLineParser::CommandLineParser( Args&&... args )
{
#ifdef __cpp_lib_logical_traits
   static_assert( ( std::is_constructible_v<std::string, Args&> && ... ) );
#endif
   ( operator<<( args ), ... );
}
#endif

CommandLineParser::CommandLineParser( int argc, char** argv ) : m_vecArgs( &argv[ 1 ], argv + argc ), m_sCommand( argv[ 0 ] )
{
}

CommandLineParser& CommandLineParser::operator<<( const std::string& arg )
{
   m_vecArgs.push_back( arg );
   return *this;
}

bool CommandLineParser::DoesSwitchExists( const std::string& name ) const noexcept
{
   if( name.empty() ) return false;

   return std::find_if(
#ifdef __cpp_lib_execution
      std::execution::par_unseq,
#endif
      m_vecArgs.cbegin(), m_vecArgs.cend(), [ name ]( const std::string& arg )->bool
      {
         return ( arg.front() == '/' || arg.front() == '-' ) && ( arg.find( name ) != std::string::npos );
      } ) != std::end( m_vecArgs );
}

std::string CommandLineParser::GetPairValue( std::string name ) const
{
   if( name.empty() ) return "";

   name += "=";
   std::string retval;
   for( const auto& pair : m_vecArgs )
   {
      const size_t switch_index = pair.find( name );
      if( switch_index != std::string::npos )
      {
         retval = pair.substr( switch_index + name.size() + 1 ); // Return string after =.
         break;
      }
   }

   return retval;
}

std::string CommandLineParser::GetNonInterpted( size_t index ) const
{
   if( m_vecArgs.size() > index ) return "";

   size_t nCounter = 0;
   std::string retval;

   for( const auto& arg : m_vecArgs )
   {
      if( arg.front() == '/' || arg.front() == '-' ) continue;
      if( ++nCounter == index ) retval = arg;
   }

   return retval;
}

CommandLineParser::ArgIterator CommandLineParser::find( const std::string& name ) const noexcept
{
   return std::find_if(
#ifdef __cpp_lib_execution
      std::execution::par_unseq,
#endif
      m_vecArgs.cbegin(), m_vecArgs.cend(), [ name ]( const std::string& arg )->bool
      {
         return arg.compare( 0, name.length(), name ) == 0;
      } );
}

std::string CommandLineParser::toLower( const std::string& str )
{
   std::string lowerCase;

   std::transform(
      str.begin(), str.end(),
      std::back_inserter( lowerCase ),
      []( const char c ) -> const char { return static_cast<char>( std::tolower( c ) ); }
   );

   return lowerCase;
}
