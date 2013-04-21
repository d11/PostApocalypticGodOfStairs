# How to build the project on Linux #

Installing prerequisites (Ubuntu example)

   sudo apt-get install git cmake make g++ libboost-thread-dev libboost-system-dev

Obtaining the code

   git clone https://github.com/d11/PostApocalypticGodOfStairs.git

Run the following commands from the build directory

    $ cmake .
    $ make

This will build the common library, libgod_common.so, and the client and server executables god_client and god_server.
