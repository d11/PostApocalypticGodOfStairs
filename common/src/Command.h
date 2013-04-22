#pragma once

#include <Data.h>

namespace god 
{

class Command
{
public:

   DataPtr execute(const std::vector<DataPtr> & args) const
   {
      std::cout << "Executing command" << std::endl;
      return Data::create();
   };

   DataPtr execute() const
   {
      std::vector<DataPtr> noArgs;
      return execute(noArgs);
   };

   virtual ~Command() {};
private:

};

}
