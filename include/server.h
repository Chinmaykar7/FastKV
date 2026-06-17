#pragma once

class Server {
public:
    explicit Server(int port);
    ~Server();

    bool start();

private:
    int port_;
    int server_fd_;
};
