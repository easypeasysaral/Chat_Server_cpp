#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

constexpr int PORT = 5555;
constexpr int BUFFER_SIZE = 1024;

void receive_messages(int socket_fd)
{
    char buffer[BUFFER_SIZE];
    while (true)
    {
        ssize_t len = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);
        if (len <= 0)
            break;
        buffer[len] = '\0';
        std::cout << buffer;
    }
}

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    std::thread(receive_messages, sock).detach();

    std::string input;
    while (std::getline(std::cin, input))
    {
        send(sock, input.c_str(), input.size(), 0);
    }

    close(sock);
    return 0;
}
