#include <iostream>

#include <chrono>
#include <thread>
#include <exec.hpp>
#include <watchdog.hpp>
#include <cstdlib>

using namespace std::chrono;



int main( ) {
   executor e{};
   
   auto pathes = get_cpu_pathes( e );
   std::size_t count = 0;
   std::size_t attempts = 0;
   for( auto x : pathes ) {
      auto attempt = set_performance_governor( e, x );
      if( attempt != 0 ) {
         count++;
      }
      attempts += attempt;
   }
   log( std::to_string(count ) + " changes applied (" + std::to_string( attempts ) + " attempts)" );

   while( true ) {
      if( am_i_gpuboehm( e ) ) {
         if( am_i_alone( e ) ) {

            //         std::system("");
         }
      }
      std::this_thread::sleep_for(10s);
   }
//   while( am_i_alone( e ) ) {
//      std::this_thread::sleep_for(1s);
//   }
   return 0;
}
