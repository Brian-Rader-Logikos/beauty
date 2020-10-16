<div align="center">
  <img alt="A Rose" src="https://github.com/dfleury2/beauty/raw/master/docs/rose.png" height="180" />
  <br>
  A simple Http server and client above <a href="https://github.com/boostorg/beast">Boost.Beast</a>
</div>
<br>

Beauty is a layer above <a href="https://github.com/boostorg/beast">Boost.Beast</a> which provide facilities to create Http server or client. Beauty allows the creation of synchronous or asynchronous server and client, and adds some signals and timer management based on <a href="https://github.com/boostorg/asio">Boost.Asio</a>

[![Build Status](https://travis-ci.org/Brian-Rader-Logikos/beauty.svg?branch=development)](https://travis-ci.org/Brian-Rader-Logikos/beauty)

## Features
- Http or Http/s server or client side
- Synchronous or Asynchronous API
- Timeout support
- Postponed response from server support
- Server easy routing with placeholders
- Timers and signals support included
- Startable and stoppable application event loop
- Customizable thread pool size

## Examples

- a server

```cpp
#include <beauty/beauty.hpp>

int main()
{
    // Create a server
    beauty::server server;

    // Add a default '/' route
    server.get("/", [](const auto& req, auto& res) {
        res.body() = "It's work ;) ... it works! :)";
    });

    // Add a '/person/:id' route
    server.get("/person/:id", [](const auto& req, auto& res) {
        auto& id = req.a("id");
        res.body() = "You asked for the person id: " + id;
    });

    // Open the listening port
    server.listen(8085);

    // Run the event loop - Warning, add a new thread (to be updated may be)
    server.run();
}

```

- a synchronous client

```cpp
#include <beauty/beauty.hpp>

#include <iostream>

int main()
{
    // Create a client
    beauty::client client;

    // Request an URL
    auto[ec, response] = client.get("http://127.0.0.1:8085");

    // Check the result
    if (!ec) {
        if (response.is_status_ok()) {
            // Display the body received
            std::cout << response.body() << std::endl;
        } else {
            std::cout << response.status() << std::endl;
        }   
    } else if (!ec) {
        // An error occurred
        std::cout << ec << ": " << ec.message() << std::endl;
    }
}
```
- an asynchronous client

```cpp
#include <beauty/beauty.hpp>

#include <iostream>
#include <chrono>

int main()
{
    // Create a client
    beauty::client client;

    // Request an URL
    client.get("http://127.0.0.1:8085",
               [](auto ec, auto&& response) {
                   // Check the result
                   if (!ec) {
                       if (response.is_status_ok()) {
                           // Display the body received
                           std::cout << response.body() << std::endl;
                       } else {
                           std::cout << response.status() << std::endl;
                       }
                   } else if (!ec) {
                       // An error occurred
                       std::cout << ec << ": " << ec.message() << std::endl;
                   }
               });

    // Need to wait a little bit to received the response
    for (int i = 0; i < 10; ++i) {
        std::cout << '.'; std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << std::endl;
}

```

- timers

```cpp
#include <beauty/beauty.hpp>

#include <iostream>

int main()
{
    // Launch a repeatable timer each 250ms
    int timer_count = 4;
    beauty::repeat(0.250, [&timer_count]() {
            std::cout << "Tick..." << std::endl;
            if (--timer_count == 0) {
                std::cout << "Dring !" << std::endl;
                beauty::stop();
            }
        });

    // Launch a one shot timer after 600ms
    beauty::after(0.600, [] {
            std::cout << "Snooze !" << std::endl;
    });

    // Wait for the end
    beauty::run();
}
```

- signals

```cpp
#include <beauty/beauty.hpp>

#include <iostream>

int main()
{
    // Catch the small signals
    beauty::signal({SIGUSR1, SIGUSR2}, [](int s) {
        std::cout << "Shot miss..." << std::endl;
    });

    // Catch the big one
    beauty::signal(SIGINT, [](int s) {
        std::cout << "Head shot !" << std::endl;
        beauty::stop();
    });

    // Wait for the end
    beauty::run();
}
```

Further examples can be found into the binaries directory at the root of the project.

## Build

Beauty depends Boost.Beast and OpenSsl. You can rely on Conan to get the package or only the FindPackage from CMake.

### Linux

```bash
git clone https://github.com/dfleury2/beauty.git
cd beauty
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

replace `cmake .. -DCMAKE_BUILD_TYPE=Release` by `cmake .. -DCMAKE_BUILD_TYPE=Release -DCONAN_false`
to not use Conan. Hope your the dependencies are found on your Linux.


### Windows
```bash
git clone https://github.com/dfleury2/beauty.git
cd beauty
mkdir build && cd build
conan install ..
cmake ..
cmake --build . --config Release
```

To be improved...
