//
// Created by jpietrzyk on 10.01.22.
//

#ifndef TUDDBS_MINING_WATCHDOG_INCLUDE_UTILS_HPP
#define TUDDBS_MINING_WATCHDOG_INCLUDE_UTILS_HPP
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <exec.hpp>

auto trim( std::string & str ) {
   std::regex r("\\s+");
   return std::regex_replace(str, r, "");
}
auto string_to_vec( std::string const & str, bool to_trim = true ) {
   std::vector< std::string > vec{};
   std::stringstream ss( str );
   while( ss.good() ) {
      std::string substr;
      std::getline( ss, substr, '\n' );
      std::string x;
      if( to_trim ) {
         x = trim( substr );
      } else {
         x = substr;
      }
      if( !x.empty() ) {
         vec.push_back( x );
      }
   }
   return vec;
}

auto get_cpu_count( executor & e ) {
   e.exec( "find /sys/devices/system/cpu/ -regextype sed -regex \".*/cpu[0-9\\-]\\+\" | wc -l " );
   return trim( e.get_output() );
}
auto get_cpu_pathes( executor & e ) {
   e.exec( "find /sys/devices/system/cpu/ -regextype sed -regex \".*/cpu[0-9\\-]\\+\"" );
   return string_to_vec( e.get_output() );
}

void log( std::string const & l ) {
   auto t = std::time(nullptr);
   auto tm = *std::localtime(&t);
   std::cout << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ": " << l << "\n";
}
auto set_performance_governor( executor & e, std::string const & path ) {
//   std::string governor = "performance";
   std::string governor = "powersave";
   std::string write_command = "echo '" + governor + "' | sudo tee " + path + "/cpufreq/scaling_governor";
   std::string read_command = "cat " + path + "/cpufreq/scaling_governor";
   e.exec( read_command.c_str() );
   std::size_t count = 0;
   while( governor != trim( e.get_output() ) ) {
      log( "Executing " + write_command );
      e.exec( write_command.c_str() );
      e.exec( read_command.c_str() );
      ++count;
   }
   return count;
}
#endif //TUDDBS_MINING_WATCHDOG_INCLUDE_UTILS_HPP
