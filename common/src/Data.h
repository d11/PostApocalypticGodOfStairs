#pragma once

#include <boost/shared_ptr.hpp>
namespace god
{

class Data;

typedef boost::shared_ptr<Data> DataPtr;

class Data
{
public:
   static DataPtr create() { return boost::shared_ptr<Data>(new Data()); }
   virtual ~Data() {};
private:
};

}
