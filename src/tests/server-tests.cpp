// // src/tests/server-tests.cpp

// #include <gtest/gtest.h>
// #include "server/Server.hpp"
// #include <thread>
// #include <chrono>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <random>
// #include <future>
// #include <vector>
// #include <sstream>

// class ServerTest : public ::testing::Test
// {
// protected:
//     const int NUM_THREADS = 4;
//     int port;
//     std::unique_ptr<Server> server;
//     std::thread serverThread;

//     void SetUp() override
//     {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<> dis(49152, 65535);
//         port = dis(gen);

//         server = std::make_unique<Server>(port, NUM_THREADS);
//         serverThread = std::thread([this]
//                                    {
//             try {
//                 server->start();
//             } catch (const std::exception& e) {
//                 std::cerr << "Server exception: " << e.what() << std::endl;
//             } });

//         // Wait for the server to start
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }

//     void TearDown() override
//     {
//         server->stop();
//         if (serverThread.joinable())
//         {
//             serverThread.join();
//         }
//     }

//     std::vector<std::string> sendSequentialRequests(const std::vector<std::string> &requests, int timeoutSeconds = 10)
//     {
//         std::promise<std::vector<std::string>> responsePromise;
//         auto responseFuture = responsePromise.get_future();

//         std::thread([&]()
//                     {
//             int sock = socket(AF_INET, SOCK_STREAM, 0);
//             if (sock < 0) {
//                 responsePromise.set_value({"Failed to create socket"});
//                 return;
//             }

//             sockaddr_in serv_addr;
//             serv_addr.sin_family = AF_INET;
//             serv_addr.sin_port = htons(port);
//             inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

//             if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//                 responsePromise.set_value({"Failed to connect to server"});
//                 close(sock);
//                 return;
//             }

//             std::vector<std::string> responses;
//             char buffer[1024] = {0};

//             // Read initial "Enter command:" prompt
//             read(sock, buffer, 1024);

//             for (const auto& request : requests) {
//                 send(sock, request.c_str(), request.length(), 0);
//                 memset(buffer, 0, sizeof(buffer));
//                 int bytesRead = read(sock, buffer, 1024);
//                 responses.push_back(std::string(buffer, bytesRead));
//             }

//             close(sock);
//             responsePromise.set_value(responses); })
//             .detach();

//         if (responseFuture.wait_for(std::chrono::seconds(timeoutSeconds)) == std::future_status::timeout)
//         {
//             return {"Test timed out"};
//         }

//         return responseFuture.get();
//     }
// };

// TEST_F(ServerTest, AddGraphAndSolveMST)
// {
//     std::vector<std::string> requests = {
//         "addGraph\n3\n0 1 2\n1 2 3\n2 0 1\n-1\n",
//         "solveMST\nKruskal\n"};

//     auto responses = sendSequentialRequests(requests);
//     std::cout << responses[0] << std::endl;
//     // ASSERT_EQ(responses.size(), 2);

//     // std::cout << "AddGraph response: " << responses[0] << std::endl;
//     // EXPECT_TRUE(responses[0].find("Graph added successfully") != std::string::npos);

//     // std::cout << "SolveMST response: " << responses[1] << std::endl;
//     // EXPECT_TRUE(responses[1].find("MST Analysis using Kruskal algorithm") != std::string::npos);
//     // EXPECT_TRUE(responses[1].find("Total weight:") != std::string::npos);
// }

// TEST_F(ServerTest, AddGraphUpdateGraphAndSolveMST)
// {
//     std::vector<std::string> requests = {
//         "addGraph\n3\n0 1 2\n1 2 3\n2 0 1\n-1\n",
//         "solveMST\nPrim\n"};

//     auto responses = sendSequentialRequests(requests);

//     ASSERT_EQ(responses.size(), 3);

//     std::cout << "AddGraph response: " << responses[0] << std::endl;
//     EXPECT_TRUE(responses[0].find("Graph added successfully") != std::string::npos);

//     std::cout << "UpdateGraph response: " << responses[1] << std::endl;
//     EXPECT_TRUE(responses[1].find("Graph updated successfully") != std::string::npos);

//     std::cout << "SolveMST response: " << responses[2] << std::endl;
//     EXPECT_TRUE(responses[2].find("MST Analysis using Prim algorithm") != std::string::npos);
//     EXPECT_TRUE(responses[2].find("Total weight:") != std::string::npos);
// }

// TEST_F(ServerTest, InvalidCommand)
// {
//     std::vector<std::string> requests = {
//         "invalidCommand\n"};

//     auto responses = sendSequentialRequests(requests);

//     ASSERT_EQ(responses.size(), 1);

//     std::cout << "InvalidCommand response: " << responses[0] << std::endl;
//     EXPECT_TRUE(responses[0].find("Unknown command") != std::string::npos);
// }

// TEST_F(ServerTest, HelpCommand)
// {
//     std::vector<std::string> requests = {
//         "help\n"};

//     auto responses = sendSequentialRequests(requests);

//     ASSERT_EQ(responses.size(), 1);

//     std::cout << "HelpCommand response: " << responses[0] << std::endl;
//     EXPECT_TRUE(responses[0].find("Available commands:") != std::string::npos);
//     EXPECT_TRUE(responses[0].find("addGraph") != std::string::npos);
//     EXPECT_TRUE(responses[0].find("updateGraph") != std::string::npos);
//     EXPECT_TRUE(responses[0].find("solveMST") != std::string::npos);
// }