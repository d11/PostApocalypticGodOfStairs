#include <iostream>
#include <boost/array.hpp>
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
         return _socket;
      }

      void readNetwork(tcp::socket & socket)
      {

         boost::array<char, 128> buf;
         boost::system::error_code error;
         boost::asio::deadline_timer timeout(_io_service);
         bool data_available = false;
         size_t len;
         std::cout << " Set async_read_some " << std::endl;
         socket.async_read_some(boost::asio::buffer(buf),
               boost::bind(&tcp_connection::handle_read, this, boost::ref(buf), boost::ref(data_available), boost::ref(timeout), error, len));


         std::cout << " Set timer for 800ms " << std::endl;
         // Clients have a fixed amount of time to make any moves, before the world updates
         timeout.expires_from_now(boost::posix_time::milliseconds(800));
         //timeout.async_wait(boost::bind(&wait_callback, boost::ref(socket),
         //      boost::asio::placeholders::error));


         std::cout << " Waiting for timer " << std::endl;
         timeout.wait();

      };

      void writeNetwork(tcp::socket & socket)
      {
         std::stringstream s;
         s << _world.getTime() << ": " << _world.getTestState() << std::endl;
         _message = s.str();
         boost::asio::async_write(socket, boost::asio::buffer(_message),
               boost::bind(&tcp_connection::handle_write, shared_from_this(),
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
      }

      void start()
      {
         for (;;)
         {
            std::cout << "Reading from clients" << std::endl;
            readNetwork(_socket);
            std::cout << "Updating world" << std::endl;
            _world.timeStep();
            std::cout << "Writing to clients" << std::endl;
            writeNetwork(_socket);
         }
      }

   private:
      tcp_connection(boost::asio::io_service& io_service, World & world)
         : _io_service(io_service), _socket(io_service), _world(world)
      {
      }

      void handle_write(const boost::system::error_code& /*error*/,
            size_t /*bytes_transferred*/)
      {
      }

      void handle_read(const boost::array<char, 128> & buf, bool & data_available, boost::asio::deadline_timer& timeout, const boost::system::error_code& error,  size_t bytesRead)
      {
         std::cout << "handle_read" << std::endl;
         if (error == boost::asio::error::eof)
         {
            //break; // Connection closed cleanly by peer.
            return;
         }
         else if (error)
         {
            throw boost::system::system_error(error); // Some other error.
         }
         std::cout << "READ : ";
         std::cout.write(buf.data(), bytesRead);
         std::cout << std::endl;
      }

      boost::asio::io_service & _io_service;
      tcp::socket _socket;
      std::string _message;
      std::stringstream _recieved;
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
