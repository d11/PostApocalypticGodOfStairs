#include <iostream>

#include "World.h"

namespace god
{
   World::World() : _worldTime(0), _testState(-1)
   {
      std::cout << "Initialising world" << std::endl;
   }
   int World::getTime() const
   {
      return _worldTime;
   }
   void World::timeStep()
   {
      _worldTime++;
   }
   void World::updateTestState(int testState)
   {
      _testState = testState;
   }
   int World::getTestState() const
   {
      return _testState;
   }
}
