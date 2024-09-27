// src/main.cpp
#include "server/Server.hpp"
#include <iostream>
#include <string>

int main() {
    Server server(4);  // Create server with 4 threads

    std::string input;
    std::cout << "MST Server started. Enter commands (or 'exit' to quit):" << std::endl;

    while (std::getline(std::cin, input)) {
        if (input == "exit") break;
        server.handleRequest(input);
    }

    return 0;
}
