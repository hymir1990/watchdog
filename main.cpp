#include <iostream>
#include <exec.hpp>
#include <chrono>
#include <thread>

using namespace std::chrono;


int main( ) {
   std::cout << "Hello, World!" << std::endl;
   executor e{};
   while( true ) {
      e.exec( "who | cut -d' ' -f1 | sort | uniq" );
      std::cout << e.get_output( ) << "\n";
      std::this_thread::sleep_for(1s);
   }
   return 0;
}
