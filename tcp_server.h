#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include <functional>

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();
    
    bool start();
    void stop();
    bool isRunning() const { return running_; }
    
    // Callback when client connects - return string to send to client
    void setRequestHandler(std::function<std::string()> handler);
    
    // Handle one client connection (blocking)
    void handleClient();

private:
    int port_;
    int server_fd_;
    bool running_;
    std::function<std::string()> request_handler_;
    
    bool setupSocket();
};

#endif
