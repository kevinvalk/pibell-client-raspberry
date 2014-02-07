#pragma once

#include "Common.h"
#include "Config.h"
#include "Action.h"

class Client
{
public:
	Client(boost::asio::io_service& ioService, tcp::resolver::iterator endpointIterator, ClientSettings settings);
	
	void addOnCall(std::shared_ptr<Action> action);
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

	// The onCall handlers
	std::list<std::function<void(bool)>> onCalls_;
};