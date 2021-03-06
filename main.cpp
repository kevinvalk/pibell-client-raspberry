#include "Common.h"
#include "Client.h"
#include "Config.h"

#define VERSION "1.0.0"

// Actions
#include "Action/Audio.h"
#ifdef RASPBERRY
	#include "Action/Glow.h"
#endif

std::shared_ptr<Client> client;

void signalHandler(int signal)
{
	client->close();
}

int main(int argc, char* argv[])
{

	std::cout << "Welcome to pibell-client version " << VERSION << "" << std::endl;

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

	// Run the client
	try
	{
		// Set up the client
		boost::asio::io_service ioService;
		tcp::resolver resolver(ioService);
		auto endpoint_iterator = resolver.resolve({ settings.hostname.empty() ? argv[1] : settings.hostname, settings.port.empty() ? argv[2] : settings.port });
		client = std::make_shared<Client>(ioService, endpoint_iterator, settings);

		// Setup audio 
		auto audio = std::make_shared<Audio>();
		audio->setFile(settings.ringtone);
		client->addOnCall(audio);

		#ifdef RASPBERRY
			// Setup glow
			auto glow = std::make_shared<Glow>(4000);
			client->addOnCall(glow);
		#endif

		// Run the client
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
