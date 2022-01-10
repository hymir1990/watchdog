//
// Created by jpietrzyk on 10.01.22.
//

#ifndef TUDDBS_MINING_WATCHDOG_INCLUDE_WATCHDOG_HPP
#define TUDDBS_MINING_WATCHDOG_INCLUDE_WATCHDOG_HPP

#include <exec.hpp>
#include <utils.hpp>



bool am_i_alone( executor & e ) {
   e.exec( "who | cut -d' ' -f1 | sort | uniq" );
   auto users = trim( e.get_output() );
   if( users.empty() ) {
      log( "Could possibly execute. No Users active");
   } else {
      // check if there are users which did something in the last seconds / minutes / hours
      e.exec(R"(w -h| sed -E -e 's/[[:blank:]]+/ /g' | grep -P '^(\S+\s){4}(\S+)\d[smh]')" );
      if( trim( e.get_output() ).empty() ) {
         log( "No users which were active in the last seconds / minutes / hours");
         // check if there are users logged in which idle since several days --> orphaned pts'
         e.exec( "w -h -s | sed -E -e 's/[[:blank:]]+/ /g' | grep \"days\" | cut -d' ' -f2" );
         auto orphaned_pts = string_to_vec( e.get_output() );
         for( const auto& orphan : orphaned_pts ) {
            auto cmd = "ps -o cmd= -ft " + orphan;
            e.exec( cmd.c_str() );
            auto commands = string_to_vec( e.get_output() );
            for( const auto& command: commands ) {
               if( "-bash" != command ) {
                  auto tmpcmd = "ps -o user= -ft " + orphan;
                  e.exec( tmpcmd.c_str() );
                  std::string l = "Could NOT execute. User " + trim( e.get_output() ) + " has an active cmd: " + command;
                  log( l );
                  return false;
               }
            }
         }
         log( "No orphaned user has a relevant cmd." );
      } else {
         auto tmp_users = string_to_vec( e.get_output(), false );
         std::string users = "";
         for( auto user : tmp_users ) {
            users += ( "{" + user + "} " );
         }
         std::string l = "Users active in the last seconds / minutes / hours (" + users + ")";
         log( l );
         return false;
      }
   }
   e.exec( R"(nvidia-smi -q -x | tr -d '\n' | sed -E -e 's/[[:blank:]]+/ /g' | grep -o -P '(?<=\<processes\>).*?(?=\<\/processes\>)' | grep -o -P '(?<=\<pid\>).*?(?=\<\/pid\>)')" );
   if( trim( e.get_output() ).empty() ) {
      log( "Could execute. No Users on gpu." );
      return true;
   } else {
      auto pids = string_to_vec( e.get_output() );
      for( auto pid : pids ) {
         std::string tmpcmd = "ps -o user= -p " + pid;
         e.exec( tmpcmd.c_str() );
         if( "root" != trim(e.get_output() ) ) {
            std::string l = "Could NOT execute. Active user on gpu (" + trim(e.get_output() ) + ")";
            log( l );
            return false;
         }
      }
      log( "Could execute. Only root on gpu." );
      return true;
   }
}

bool am_i_gpuboehm( executor & e ) {
   e.exec( "hostname" );
   auto users = trim( e.get_output() );
   return ( "gpuboehm" == users );
}
#endif //TUDDBS_MINING_WATCHDOG_INCLUDE_WATCHDOG_HPP
