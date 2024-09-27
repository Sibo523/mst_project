// tests/ServerTests.cpp
#include "../src/server/Server.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

// Helper function to capture cout
class CoutCapture {
    std::stringstream buffer;
    std::streambuf* old;
public:
    CoutCapture() : old(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CoutCapture() { std::cout.rdbuf(old); }
    std::string getOutput() { return buffer.str(); }
};

void testAddGraph() {
    Server server(1);
    CoutCapture capture;
    
    server.handleRequest("addGraph 4 0 1 10 0 2 6 0 3 5 1 3 15 2 3 4");
    
    // Wait for asynchronous operation to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    assert(capture.getOutput().find("Graph added successfully") != std::string::npos);
    std::cout << "Add graph test passed.\n";
}

void testUpdateGraph() {
    Server server(1);
    CoutCapture capture;
    
    server.handleRequest("addGraph 4 0 1 10 0 2 6 0 3 5 1 3 15 2 3 4");
    server.handleRequest("updateGraph 0 1 20");
    
    // Wait for asynchronous operations to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    assert(capture.getOutput().find("Graph updated successfully") != std::string::npos);
    std::cout << "Update graph test passed.\n";
}

void testSolveMST() {
    Server server(1);
    CoutCapture capture;
    
    server.handleRequest("addGraph 4 0 1 10 0 2 6 0 3 5 1 3 15 2 3 4");
    server.handleRequest("solveMST Prim");
    
    // Wait for asynchronous operations to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::string output = capture.getOutput();
    assert(output.find("MST Analysis using Prim algorithm") != std::string::npos);
    assert(output.find("Total weight: 15") != std::string::npos);
    std::cout << "Solve MST test passed.\n";
}

void testUnknownCommand() {
    Server server(1);
    CoutCapture capture;
    
    server.handleRequest("unknownCommand");
    
    // Wait for asynchronous operation to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    assert(capture.getOutput().find("Unknown command") != std::string::npos);
    std::cout << "Unknown command test passed.\n";
}

// int main() {
//     testAddGraph();
//     testUpdateGraph();
//     testSolveMST();
//     testUnknownCommand();

//     std::cout << "All Server tests passed!\n";
//     return 0;
// }
