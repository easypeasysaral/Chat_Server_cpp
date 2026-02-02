#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr int PORT = 5555;
constexpr int BUFFER_SIZE = 1024;

std::unordered_map<std::string, int> clients; // username -> socket
std::mutex clients_mutex;

/* Utility: send message safely */
void send_message(int client_socket, const std::string &msg)
{
    send(client_socket, msg.c_str(), msg.size(), 0);
}

/* Broadcast to all clients except sender */
void broadcast(const std::string &sender, const std::string &message)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto &[username, sock] : clients)
    {
        if (username != sender)
        {
            send_message(sock, sender + ": " + message + "\n");
        }
    }
}

/* Private messaging */
void private_message(const std::string &sender,
                     const std::string &target,
                     const std::string &message)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    if (clients.count(target))
    {
        send_message(clients[target],
                     "[PM from " + sender + "]: " + message + "\n");
    }
    else
    {
        send_message(clients[sender],
                     "User not found: " + target + "\n");
    }
}

/* Handle each connected client */
void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];
    std::string username;

    send_message(client_socket, "Enter username: ");

    ssize_t len = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (len <= 0)
    {
        close(client_socket);
        return;
    }

    buffer[len] = '\0';
    username = std::string(buffer);
    username.erase(std::remove(username.begin(), username.end(), '\n'),
                   username.end());

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        if (clients.count(username))
        {
            send_message(client_socket, "Username already taken.\n");
            close(client_socket);
            return;
        }
        clients[username] = client_socket;
    }

    std::cout << "[+] User connected: " << username << std::endl;
    broadcast("Server", username + " joined the chat");

    while (true)
    {
        len = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (len <= 0)
            break;

        buffer[len] = '\0';
        std::string msg(buffer);
        msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());

        if (msg.rfind("/msg", 0) == 0)
        {
            std::istringstream iss(msg);
            std::string cmd, target, text;
            iss >> cmd >> target;
            std::getline(iss, text);
            private_message(username, target, text);
        }
        else
        {
            broadcast(username, msg);
        }
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(username);
    }

    broadcast("Server", username + " left the chat");
    std::cout << "[-] User disconnected: " << username << std::endl;

    close(client_socket);
}

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Socket failed");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) < 0)
    {
        perror("Listen failed");
        return 1;
    }

    std::cout << "Chat server running on port " << PORT << std::endl;

    while (true)
    {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0)
            continue;

        std::thread(handle_client, client_socket).detach();
    }

    close(server_socket);
    return 0;
}
