#include "Client.h"

Client::Client(boost::asio::io_service& ioService, tcp::resolver::iterator endpointIterator, ClientSettings settings)
	: ioService_(ioService), socket_(ioService)
{
	// Initialize
	settings_ = settings;

	// Connect our client
	doConnect(endpointIterator);
}

void Client::addOnCall(std::shared_ptr<Action> action)
{
	onCalls_.push_back(action->getOnCall());
}

void Client::send(const Packet &packet)
{
	ioService_.post(
		[this, packet]()
		{
			bool isSending = ! sends_.empty();
			sends_.push_back(std::move(packet));
			if ( ! isSending)
				doSend();
		}
	);
}

void Client::close()
{
	// Nicely send exit to server before closing
	PacketClose close;
	boost::asio::write(socket_, boost::asio::buffer((PacketCast close).getData(), (PacketCast close).getTotalLength()));

	// Close the socket
	ioService_.post([this]() { socket_.close(); });
}

void Client::handleCall()
{
	auto p = reinterpret_cast<PacketCall*>(&receive_);
	std::cout << "Received call for " << (p->global ? "all" : "me") << " @ " << std::asctime(std::localtime(&p->time));
	
	// Call all onCalls
	for (auto onCall : onCalls_)
	{
		onCall(p->global);
	}
}

void Client::handleReceive()
{
	boost::asio::async_read(
		socket_,
		boost::asio::buffer(receive_.getBody(), receive_.getLength()),
		[this](boost::system::error_code ec, std::size_t)
		{
			if ( ! ec)
			{
				
				switch (receive_.getType())
				{
					case PacketType::STATE:
						std::cout << "Received state packet" << std::endl;
					break;
					case PacketType::CALL:
						handleCall();
					break;
					default:
						std::cout << "Received unhandeled packet with type:" << receive_.getType() << std::endl;
					break;
				}

				// Keep waiting for new stuff
				doReceive();
			}
			else
			{
				socket_.close();
			}
		}
	);
}

void Client::doReceive()
{
	boost::asio::async_read(
		socket_,
		boost::asio::buffer(receive_.getData(), Packet::headerLength),
		[this](boost::system::error_code ec, std::size_t)
		{
			if ( ! ec)
				handleReceive();
			else
				socket_.close();
		}
	);
}

void Client::doSend()
{
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(sends_.front().getData(), sends_.front().getTotalLength()),
		[this](boost::system::error_code ec, std::size_t)
		{
			if ( ! ec)
			{
				sends_.pop_front();
				if ( ! sends_.empty())
					doSend();
			}
			else
				socket_.close();
		}
	);
}

void Client::doConnect(tcp::resolver::iterator endpointIterator)
{
	boost::asio::async_connect(
		socket_,
		endpointIterator,
		[this](boost::system::error_code ec, tcp::resolver::iterator)
		{
			if ( ! ec)
			{
				std::cout << "Connected" << std::endl;
				PacketRegister p;
				p.global = true;
				p.username.set(settings_.username);
				p.password.set(settings_.password);
				send(PacketCast p);
				
				// Start our receiving cycle
				doReceive();
			}
		}
	);
}
