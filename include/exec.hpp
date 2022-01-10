//
// Created by jpietrzyk on 10.01.22.
//

#ifndef TUDDBS_MINING__EXEC_HPP
#define TUDDBS_MINING__EXEC_HPP


#include <istream>
#include <streambuf>
#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

class executor {
   protected:
      std::string output;
      std::array< char, 128 > buffer;
   public:
      executor(): output{}, buffer{} { }
   public:
      void exec( char const * command ) {
         this->output = "";
         std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
         if (!pipe) {
            throw std::runtime_error("popen() failed!");
         }
         while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            this->output += buffer.data();
         }
      }
      std::string & get_output() {
         return output;
      }
      
};

#endif //TUDDBS_MINING__EXEC_HPP
