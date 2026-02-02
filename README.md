## C++ Multi-Client Chat Server
A high-performance multi-client chat server built using C++, TCP socket programming, and multi-threading, designed to demonstrate practical knowledge of network programming, concurrent systems, and server-side architecture.
The system enables multiple users to connect simultaneously and exchange messages in real time through reliable TCP communication.
This project showcases core backend engineering skills relevant to scalable communication systems.

## Project Overview
This application implements a concurrent chat server capable of handling multiple clients simultaneously. Each client connection runs in a separate thread, enabling real-time communication without blocking other users.
The project focuses on:
•	Network communication over TCP
•	Concurrent client handling
•	Thread-safe resource management
•	Server-side architecture design
•	Real-time message broadcasting

## Key Features
•	Multi-client support using concurrent threading
•	Real-time message broadcasting
•	Reliable TCP/IP communication
•	Graceful handling of client connection and disconnection
•	Thread-safe shared resource access
•	Scalable and modular server design
•	Robust error handling for network failures

## Technology Stack
| Component              | Technology                    |
|------------------------|-------------------------------|
| Programming Language   | C++                           |
| Networking             | TCP/IP Socket Programming     |
| Concurrency            | `std::thread`, `std::mutex`  |
| Operating System       | Linux / Unix-based            |
| Compiler               | g++                           |

## Project Structure

C++-ChatServer/
│
├── main.cpp        # Server-side implementation
├── client.cpp      # Client application



## System Architecture (High-Level)

Client 1 ─┐
Client 2 ─┼──> Server Socket ──> Thread per Client ──> Message Broadcast
Client 3 ─┘


## Installation & Execution
## Clone Repository
git clone https://github.com/your-username/C++-ChatServer.git
cd C++-ChatServer
## Compile
g++ server.cpp -o server -pthread
g++ client.cpp -o client
## Run Server
./server
## Run Clients (multiple terminals)
./client
Clients can now connect and exchange messages.

## System Architecture Overview
•	Server creates a TCP socket and listens for connections.
•	Clients connect via TCP sockets.
•	Each client is handled in an independent thread.
•	Incoming messages are broadcast to all active clients.
•	Clients are safely removed when disconnected.

## Technical Concepts Demonstrated

## Socket Programming
Inter-process communication is implemented using TCP sockets, enabling reliable network communication using IP addresses and ports.
System calls used:
socket()
bind()
listen()
accept()
send()
recv()
close()

## Concurrent Client Handling
Each client runs in its own thread, ensuring non-blocking communication.
Example:
std::thread clientThread(handleClient, clientSocket);
clientThread.detach();

## Thread Synchronization
Shared resources such as connected client lists are protected using mutex locks to avoid race conditions.
Example:
std::mutex clientMutex;

## Message Broadcasting
Messages received from one client are forwarded to all connected clients, enabling group communication.

## Error Handling & Stability
•	The server handles:
•	Socket creation failures
•	Client disconnections
•	Transmission errors
•	Port binding issues
This ensures server stability under typical network failures.

## Engineering Skills Demonstrated
This project demonstrates practical experience with:
•	Concurrent server design
•	TCP/IP networking
•	Multi-threaded programming in C++
•	Resource synchronization techniques
•	Real-time communication systems
•	Backend architecture principles



<img width="1451" height="748" alt="image" src="https://github.com/user-attachments/assets/d662f561-c333-4975-8acc-47f1526af7dc" />





<img width="1479" height="763" alt="Screenshot 2026-02-03 025725" src="https://github.com/user-attachments/assets/d7345c9c-182f-4727-9c30-30051c2102ec" />




<img width="1429" height="586" alt="Screenshot 2026-02-03 025757" src="https://github.com/user-attachments/assets/5eeac149-1a73-4a0b-b918-028e6eeca988" />


