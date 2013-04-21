#pragma once
#include <defines.h>

namespace god
{
#if _WIN32
   class DllExport World
#else
   class World
#endif
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
