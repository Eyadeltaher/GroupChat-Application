# Group Chat Application in C

This is a **Group Chat Application** implemented in C, designed to facilitate real-time communication between multiple users over a network. The application consists of a **multi-threaded server** and **multiple clients**, allowing users to join a chat room, send messages, and receive messages from other users in real-time.

---

## Features

- **Multi-Threaded Server**: Handles multiple client connections simultaneously using threads.
- **Real-Time Messaging**: Clients can send and receive messages in real-time.
- **Broadcast Messaging**: Messages sent by one client are broadcast to all other connected clients.
- **Graceful Disconnection**: Clients can disconnect gracefully by typing `"exit"`.

---

## How It Works

### Server
- Listens for incoming client connections on a specified port.
- Creates a new thread for each connected client to handle communication.
- Broadcasts messages received from one client to all other connected clients.

### Client
- Connects to the server using the server's IP address and port.
- Sends messages to the server, which are broadcast to all clients.
- Receives messages from other clients in real-time.

---

## Technologies Used

- **C Programming Language**: Core implementation of the application.
- **Socket Programming**: For network communication between the server and clients.
- **Multi-Threading**: To handle multiple clients concurrently.

---

## How to Run

### Prerequisites
- A C compiler (e.g., `gcc`).
- POSIX-compliant operating system (e.g., Linux, macOS).

- ### Steps

1. **Clone the Repository**:
   bash
   git clone https://github.com/Eyadeltaher/group-chat-application-c.git
   cd group-chat-application-c

2. **Compile the Server**:
    bash
    ```gcc SocketServer.c -o server -lpthread```

3.  **Run the Server**:
    bash
    ```./server```

4.  **Compile the Client**:
    bash
    ```gcc socketclient.c -o client -lpthread```

5.  **Run the Client**:
    bash
    ```./client```

6.  **Connect Multiple Clients**:
       Open multiple terminal windows and run the client program in each window.
       Start chatting in real-time!
