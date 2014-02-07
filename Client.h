#pragma once

#include "Common.h"
#include "Config.h"
#include "Audio.h"

class Client
{
public:
	Client(boost::asio::io_service& ioService, tcp::resolver::iterator endpointIterator, std::shared_ptr<Audio> audio, ClientSettings settings);
	
	void send(const Packet &packet);
	void close();

private:
	void doConnect(tcp::resolver::iterator endpointIterator);
	void doReceive();
	void handleReceive();
	void doSend();

	// Specialized handlers
	void handleCall();

	// Variables
	boost::asio::io_service& ioService_;
	tcp::socket socket_;
	Packet receive_;
	Packet send_;
	PacketQueue sends_;

	ClientSettings settings_;
	std::shared_ptr<Audio> audio_;
};