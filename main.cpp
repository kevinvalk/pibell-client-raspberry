#include "Common.h"
#include "Audio.h"
#include "Client.h"
#include "Config.h"


std::shared_ptr<Client> client;

void signalHandler(int signal)
{
	client->close();
}

int main(int argc, char* argv[])
{
	std::cout << "Welcome to pibell client for " << PLATFORM << "!" << std::endl;

	// Install our signal handlers to correctly exit the client
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);

	// Read the config
	ClientSettings settings;
	try
	{
		settings = Config::getSettings("config.ini");
	}
	catch (std::exception &e)
	{
		std::cerr << "Config error:" << e.what() << std::endl;
		exit(1);
	}

	// Check for config 
	if ((settings.hostname.empty() && (argc < 2 || std::strlen(argv[1]) <= 0)) || (settings.port.empty() && (argc < 3 || std::strlen(argv[2]) <= 0)))
	{
		std::cerr << argv[0] << " [<hostname> [<port>]]" << std::endl << "Or set `hostname` en `port` in config.ini" << std::endl;
		exit(1);
	}
	if (settings.username.empty() || settings.password.empty())
	{
		std::cerr << "Yo do not have a username or/and password set in config.ini" << std::endl;
		exit(1);
	}

	// Initialize the audio
	auto audio = std::make_shared<Audio>();

	// Run the client
	try
	{
		// Set up the server
		boost::asio::io_service ioService;

		tcp::resolver resolver(ioService);
		auto endpoint_iterator = resolver.resolve({ settings.hostname.empty() ? argv[1] : settings.hostname, settings.port.empty() ? argv[2] : settings.port });
		client = std::make_shared<Client>(ioService, endpoint_iterator, audio, settings);

		// Run the server
		std::thread t([&ioService](){ ioService.run(); });

		// Wait for thread to terminate
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		exit(1);
	}


	// We are done !
	std::cout << "Goodbye" << std::endl;
	return 0;
}
