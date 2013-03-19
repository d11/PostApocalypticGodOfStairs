#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <config.h>
#include <World.h>

using boost::asio::ip::tcp;
using namespace god;

std::string make_daytime_string()
{
   using namespace std; // For time_t, time and ctime;
   time_t now = time(0);
   return ctime(&now);
}

class tcp_connection
: public boost::enable_shared_from_this<tcp_connection>
{
   public:
      typedef boost::shared_ptr<tcp_connection> pointer;

      static pointer create(boost::asio::io_service& io_service, World & world)
      {
         return pointer(new tcp_connection(io_service, world));
      }

      tcp::socket& socket()
      {
         return socket_;
      }

      void readNetwork()
      {

      };

      void writeNetwork()
      {
         std::stringstream s;
         s << _world.getTime() << ": " << _world.getTestState();
         message_ = s.str();
         boost::asio::async_write(socket_, boost::asio::buffer(message_),
               boost::bind(&tcp_connection::handle_write, shared_from_this(),
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
      }

      void start()
      {
         for (;;)
         {
            readNetwork();
            _world.timeStep();
            writeNetwork();
         }
      }

   private:
      tcp_connection(boost::asio::io_service& io_service, World & world)
         : socket_(io_service), _world(world)
      {
      }

      void handle_write(const boost::system::error_code& /*error*/,
            size_t /*bytes_transferred*/)
      {
      }

      tcp::socket socket_;
      std::string message_;
      World & _world;
};

class tcp_server
{
   public:
      tcp_server(boost::asio::io_service& io_service, int port, World & world )
         : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), _world(world)
      {
         start_accept();
      }

   private:
      void start_accept()
      {
         tcp_connection::pointer new_connection =
            tcp_connection::create(acceptor_.get_io_service(), _world);

         acceptor_.async_accept(new_connection->socket(),
               boost::bind(&tcp_server::handle_accept, this, new_connection,
                  boost::asio::placeholders::error));
      }

      void handle_accept(tcp_connection::pointer new_connection,
            const boost::system::error_code& error)
      {
         if (!error)
         {
            new_connection->start();
         }

         start_accept();
      }

      tcp::acceptor acceptor_;
      World & _world;
};

int main(int argc, char *argv[])
{
   const int port = 16008;
   std::cout << "Starting server (version " << VERSION_STRING << ") on port " << port << std::endl;

   World world;

   try
   {
      boost::asio::io_service io_service;
      tcp_server server(io_service, port, world);
      boost::thread networkThread(boost::bind(&boost::asio::io_service::run, &io_service));
      networkThread.join();
   }
   catch (std::exception& e)
   {
      std::cerr << e.what() << std::endl;
   }

   return 0;
}
