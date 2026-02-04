#include "tcp_server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

TCPServer::TCPServer(int port) 
    : port_(port), server_fd_(-1), running_(false) {
}

TCPServer::~TCPServer() {
    stop();
}

void TCPServer::setRequestHandler(std::function<std::string()> handler) {
    request_handler_ = handler;
}

bool TCPServer::setupSocket() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    // Allow port reuse
    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed\n";
        close(server_fd_);
        return false;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed on port " << port_ << "\n";
        close(server_fd_);
        return false;
    }

    if (listen(server_fd_, 3) < 0) {
        std::cerr << "Listen failed\n";
        close(server_fd_);
        return false;
    }

    return true;
}

bool TCPServer::start() {
    if (running_) {
        return true;
    }

    if (!setupSocket()) {
        return false;
    }

    running_ = true;
    std::cout << "Server listening on port " << port_ << "\n";
    return true;
}

void TCPServer::stop() {
    if (server_fd_ >= 0) {
        close(server_fd_);
        server_fd_ = -1;
    }
    running_ = false;
}

void TCPServer::handleClient() {
    if (!running_) {
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        return;
    }

    std::cout << "Client connected from " 
              << inet_ntoa(client_addr.sin_addr) << "\n";

    // Get response from handler
    std::string response;
    if (request_handler_) {
        response = request_handler_();
    } else {
        response = "No handler set\n";
    }

    // Send response
    send(client_fd, response.c_str(), response.length(), 0);
    
    close(client_fd);
    std::cout << "Client disconnected\n";
}

