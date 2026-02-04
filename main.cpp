#include "network_stats.h"
#include "tcp_server.h"
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic<bool> keep_running(true);

void signalHandler(int signum) {
    std::cout << "\nReceived signal " << signum << ", shutting down...\n";
    keep_running = false;
}

int main(int argc, char* argv[]) {
    std::cout << "Network Monitor Starting...\n";
    
    // Setup signal handlers for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create network stats reader
    NetworkStats netStats;
    
    // Create TCP server on port 8080
    TCPServer server(8080);
    
    // Set handler to return current network stats
    server.setRequestHandler([&netStats]() {
        netStats.update();
        return netStats.formatStats();
    });

    if (!server.start()) {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    std::cout << "Network monitor ready!\n";
    std::cout << "Connect with: telnet <board-ip> 8080\n";
    std::cout << "Press Ctrl+C to stop\n\n";

    // Main loop: handle client connections
    while (keep_running) {
        server.handleClient();
        // Small sleep to prevent tight loop when no clients
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    server.stop();
    std::cout << "Server stopped. Goodbye!\n";
    
    return 0;
}
