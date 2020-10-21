#include <beauty/beauty.hpp>
#include <iostream>

int main()
{
    // Catch the small signals
    beauty::signal({SIGUSR1, SIGUSR2}, [](int s) {
        std::cout << "Shot miss..." << std::endl;
        beauty::stop();
    });

    // Catch the big one
    beauty::signal(SIGINT, [](int s) {
        std::cout << "Head shot !" << std::endl;
        beauty::stop();
    });

    // Create a server
    beauty::server server;

    // Add a default '/' route
    server.get("/", [](const auto& req, auto& res) {
        res.body() = "Default";
    });

    // Open the listening port
    server.listen(49152);
    std::cout << "Listening on port 49152\n";

    // Run the event loop - Warning, add a new thread (to be updated may be)
    server.run();
}
