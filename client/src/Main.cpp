#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <World.h>
#include <config.h>

using boost::asio::ip::tcp;

using namespace god;

void runClient(tcp::socket & socket)
{
   for (;;)
   {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
         break; // Connection closed cleanly by peer.
      else if (error)
         throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
      std::cout << std::endl;
   }
}

int main(int argc, char *argv[])
{
   std::cout << "Starting client (version " << VERSION_STRING << ")" << std::endl;
   const int port = 16008;
   World world;

   try
   {
      if (argc != 2)
      {
         std::cerr << "Usage: client <host>" << std::endl;
         return 1;
      }

      boost::asio::io_service io_service;

      tcp::resolver resolver(io_service);
      tcp::resolver::query query(argv[1], std::to_string(port));
      tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
      tcp::resolver::iterator end;

      tcp::socket socket(io_service);
      boost::system::error_code error = boost::asio::error::host_not_found;
      while (error && endpoint_iterator != end)
      {
         socket.close();
         socket.connect(*endpoint_iterator++, error);
      }
      if (error)
         throw boost::system::system_error(error);

      boost::thread networkThread(boost::bind(&boost::asio::io_service::run, &io_service));
      runClient(socket);
      char line[100];
      while (std::cin.getline(line, 100))
      {
         std::cout << line << std::endl;
      }
      networkThread.join();
   }
   catch (std::exception& e)
   {
      std::cerr << e.what() << std::endl;
   }

   return 0;
}
