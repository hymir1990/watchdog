#include <iostream>
#include <exec.hpp>
#include <chrono>
#include <thread>

using namespace std::chrono;


int main( ) {
   executor e{};
   
   
   e.exec("hostname");
//   if( "p52s" == e.get_output() ) {
//      e.exec( "who | cut -d' ' -f1 | sort | uniq" );
//
//   }
   while( true ) {
      e.exec( "who | cut -d' ' -f1 | sort | uniq" );
      
      if( e.get_output().length() == 0 ) {
         std::cout << "NONE\n";
      } else {
         std::cout << e.get_output( ) << "\n";
      }
      std::this_thread::sleep_for(1s);
   }
   return 0;
}
