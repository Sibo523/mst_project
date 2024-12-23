#include "Server.hpp"
#include "../factory/MSTFactory.hpp"
#include "../analysis/MSTAnalysis.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <time.h>

Server::Server(int port, int threads)
    : currentGraph(0),
      threadPool(threads),
      pipeline(MSTAnalysis::getPipelineFunctions()),
      port(port),
      running(false)
{
    pipeline.start();
}

void Server::start()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Failed to create socket\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Set SO_REUSEADDR option
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "Failed to set SO_REUSEADDR option\n";
        return;
    }

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Failed to bind to port " << port << "\n";
        return;
    }

    if (listen(serverSocket, 3) < 0)
    {
        std::cerr << "Failed to listen on socket\n";
        return;
    }

    std::cout << "Server is listening on port " << port << "\n";

    // Initialize and run the proactor with the server socket
    proactor.run(serverSocket, [this](int clientSocket)
                 { this->handleClient(clientSocket); });

    // Wait for the proactor thread
    if (auto thread = proactor.get_thread())
    {
        thread->join();
    }
}

void Server::stop()
{
    running = false;
    proactor.stop();
    close(serverSocket);
}

// Rest of the implementation remains the same...
void Server::handleClient(int clientSocket)
{
    std::cout << "got to here";
    char buffer[1024] = {0};
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        sendMessage(clientSocket, "Enter command:\n addGraph,\n solveMST \n updateGraph \n exit\n clear \n help\n");

        int valread = read(clientSocket, buffer, 1024);
        if (valread <= 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }
        std::string request(buffer);
        std::cout << "Received request: " << request << std::endl;

        if (request == "exit\n")
        {
            std::cout << "Client requested exit\n";
            break;
        }

        std::string response = handleRequest(request, clientSocket);
        sendMessage(clientSocket, response);
    }
    close(clientSocket);
}

// handle the requests of the user and return the response
std::string Server::handleRequest(const std::string &request, int clientSocket)
{
    std::istringstream iss(request);
    std::string command;
    iss >> command;

    if (command == "addGraph")
    {
        sendMessage(clientSocket, "Enter the number of vertices:");
        std::string input = getClientInput(clientSocket);
        int V = std::stoi(input);
        Graph newGraph(V);

        while (true)
        {
            sendMessage(clientSocket, "Enter edges (src dest weight), '-1' when finished:\n");
            input = getClientInput(clientSocket);
            if (input == "-1" || input == "-1\n" || input == "-1\r\n")
                break;

            std::istringstream edgeStream(input);
            int src, dest, weight;
            if (edgeStream >> src >> dest >> weight)
            {
                try
                {
                    newGraph.addEdge(src, dest, weight);
                    sendMessage(clientSocket, "Edge added successfully. src: " + std::to_string(src) + ", dest: " + std::to_string(dest) + ", weight: " + std::to_string(weight) + "\n");
                }
                catch (const std::exception &e)
                {
                    sendMessage(clientSocket, e.what() + '\n');
                }
            }
            else
            {
                sendMessage(clientSocket, "Invalid input. Try again or type 'done':\n");
            }
        }

        addGraph(newGraph);
        return "Graph added successfully.\n";
    }
    else if (command == "updateGraph")
    {
        sendMessage(clientSocket, "Enter graph changes:");
        std::string changes = getClientInput(clientSocket);
        updateGraph(changes);
        return "Graph updated successfully.";
    }
    else if (command == "solveMST")
    {
        sendMessage(clientSocket, "Enter MST algorithm (e.g., 'Kruskal', 'Prim', 'Tarjan', 'Boruvka'):");
        std::string algorithm = getClientInput(clientSocket);
        algorithm = trimString(algorithm); // Remove any leading/trailing whitespace
        std::cout << "Algorithm received: '" << algorithm << "'" << std::endl;

        std::vector<std::string> validAlgorithms = {"Kruskal", "Prim", "Tarjan", "Boruvka"};
        if (std::find(validAlgorithms.begin(), validAlgorithms.end(), algorithm) != validAlgorithms.end())
        {
            std::cout << "Solving MST with algorithm: " << algorithm << std::endl;
            std::cout << currentGraph.getVertices() << std::endl;
            sendMessage(clientSocket, "do you want to use leaderfollower or pipeline? 1 for leaderfollower, 2 for pipeline\n");
            // reveive the choice of the user and solve the mst
            std::string choice = getClientInput(clientSocket);
            choice = trimString(choice);
            std::string result = solveMST(algorithm, choice[0] - '0', clientSocket);
            std::cout << "MST result: " << result << std::endl;
            return result;
        }
        else
        {
            std::string errorMsg = "Invalid MST algorithm: '" + algorithm + "'. Valid options are: Kruskal, Prim, Tarjan, Boruvka.\n";
            sendMessage(clientSocket, errorMsg);
            return errorMsg;
        }
    }
    else if (command == "exit")
    {
        stop();
        return "Server stopped.";
    }
    else if (command == "help")
    {
        std::string helpMsg = "Available commands:\n";
        helpMsg += "addGraph: Add a new graph\n";
        helpMsg += "updateGraph: Update the current graph\n";
        helpMsg += "solveMST: Solve the Minimum Spanning Tree problem\n";
        helpMsg += "exit: Stop the server\n";
        return helpMsg;
    }
    else if (command == "clear")
    {
        graphMutex.lock();
        currentGraph = Graph(0);
    }

    else
    {
        return "Unknown command: " + command;
    }
}

// Helper function to trim whitespace from a string (copied from https://stackoverflow.com/a/217605)
std::string trimString(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}
// send the message to the client easily managed
void Server::sendMessage(int clientSocket, const std::string &message)
{
    send(clientSocket, message.c_str(), message.length(), 0);
}
// get the input from the client easily mangaged
std::string Server::getClientInput(int clientSocket)
{
    char buffer[1024] = {0};
    int valread = read(clientSocket, buffer, 1024);
    if (valread <= 0)
    {
        return "";
    }
    return std::string(buffer);
}
// overide the old graph, and print the message
void Server::addGraph(const Graph &graph)
{
    {
        std::lock_guard<std::mutex> lock(graphMutex);
        currentGraph = graph;
        std::cout << "Graph added successfully." << std::endl;
    }
}
// update the current graph
void Server::updateGraph(const std::string &changes)
{
    {
        std::lock_guard<std::mutex> lock(graphMutex);
        std::istringstream iss(changes);
        int src, dest, weight;
        while (iss >> src >> dest >> weight)
        {
            currentGraph.addEdge(src, dest, weight);
        }
    }
    std::cout << "Graph updated successfully." << std::endl;
}
// solve the MST problem with the given algorithm
// main part of the server/project
std::string Server::solveMST(const std::string &algorithm, int choice, int clientSocket)
{
    std::cout << "Solving MST with algorithm: " << algorithm << std::endl;

    auto mstAlgo = MSTFactory::createMSTAlgorithm(algorithm);
    if (!mstAlgo)
    {
        std::cerr << "Failed to create MST algorithm: " << algorithm << std::endl;
        return "Error: Failed to create MST algorithm.";
    }
    std::cout << "before findMST" << std::endl;
    std::vector<std::pair<int, std::pair<int, int>>> mst;
    try
    {
        mst = mstAlgo->findMST(currentGraph);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in findMST: " << e.what() << std::endl;
        return "Error: Exception occurred while finding MST.";
    }

    try
    {
        // we are going of the choice of the user
        std::string result;
        switch (choice)
        {
        case 1:
            // LeaderFollower
            result = threadPool.processMST(mst);
            break;

        case 2:
            // Pipeline
            result = pipeline.execute(mst);
            break;

        default:
            result = "Error: wrong choice propebly";
            break;
        }

        return result;
    }
    catch (const std::exception &e)
    {
        std::string choice_str = choice == 1 ? "LeaderFollower" : "Pipeline";
        std::cerr << "Exception in " << choice_str << " processing: " << e.what() << std::endl;
        return "Error: Exception occurred while processing MST result.";
    }
}