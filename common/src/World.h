#pragma once

namespace god
{
   class World
   {
   private:
      int _worldTime;
      int _testState;
   public:
      World();
      void timeStep();
      int getTime() const;
      void updateTestState(int testState);
      int getTestState() const;
   };
}
