#include <gtest/gtest.h>
#include "../utils/Proactor.hpp"
#include <thread>
#include <chrono>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class ProactorTest : public ::testing::Test {
protected:
    std::unique_ptr<Proactor> proactor;
    int serverSocket;

    void SetUp() override {
        proactor = std::make_unique<Proactor>();
        
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        ASSERT_GE(serverSocket, 0);

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(0);  // Let system choose port

        ASSERT_GE(bind(serverSocket, (struct sockaddr*)&address, sizeof(address)), 0);
        ASSERT_GE(listen(serverSocket, 3), 0);
    }

    void TearDown() override {
        proactor->stop();
        close(serverSocket);
    }
};

TEST_F(ProactorTest, BasicOperation) {
    std::atomic<bool> handlerCalled{false};
    
    auto handler = [&handlerCalled](int socket) {
        handlerCalled = true;
        close(socket);
    };

    proactor->run(serverSocket, handler);
    
    // Create client connection
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    socklen_t len = sizeof(address);
    getsockname(serverSocket, (struct sockaddr*)&address, &len);
    
    connect(clientSocket, (struct sockaddr*)&address, sizeof(address));
    
    // Wait for handler to be called
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    ASSERT_TRUE(handlerCalled);
    close(clientSocket);
}

TEST_F(ProactorTest, MultipleConnections) {
    std::atomic<int> connectionCount{0};
    
    auto handler = [&connectionCount](int socket) {
        connectionCount++;
        close(socket);
    };

    proactor->run(serverSocket, handler);

    const int NUM_CONNECTIONS = 5;
    std::vector<std::thread> clients;
    
    sockaddr_in address;
    socklen_t len = sizeof(address);
    getsockname(serverSocket, (struct sockaddr*)&address, &len);

    for(int i = 0; i < NUM_CONNECTIONS; i++) {
        clients.emplace_back([&address] {
            int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
            connect(clientSocket, (struct sockaddr*)&address, sizeof(address));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            close(clientSocket);
        });
    }

    for(auto& client : clients) {
        client.join();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_EQ(connectionCount, NUM_CONNECTIONS);
}


TEST_F(ProactorTest, ThreadCleanup) {
    auto handler = [](int socket) {
        close(socket);
    };

    proactor->run(serverSocket, handler);
    
    // Start and stop multiple times to test thread cleanup
    for(int i = 0; i < 3; i++) {
        proactor->stop();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        proactor->run(serverSocket, handler);
    }
    
    // Verify the thread is still accessible
    ASSERT_NE(proactor->get_thread(), nullptr);
}