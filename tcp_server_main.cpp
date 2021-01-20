
#include <iostream>
#include <string>
#include <vector>

#include "RequestParser.h"
#include "message_board.h"

#include "include/tcp_server.h"

// declare the server
TcpServer server;

// declare the message board
message_board my_message_board;

// declare a server observer which will receive incoming messages.
// the server supports multiple observers
server_observer_t message_board_observer;

// observer callback. will be called for every new message received by clients
// with the requested IP address
void process_incoming_message(const Client *client, const char *msg, size_t size) {
    std::string msgStr = msg;

    // Check if the incoming request is a POST Request
    if (msgStr.find("POST") != std::string::npos)
    {
        // Process the post request and send the response to the Client
        PostRequest post = PostRequest::parse(msgStr);
        if (post.valid)
        {
            unsigned int postId = my_message_board.Post(post.getTopicId(), post.getMessage());
            // Send the response to the client
            server.sendToClient(client, std::to_string(postId).c_str(), std::to_string(postId).length());
        }
    }

    // Check if the incoming request is a LIST Request
    else if (msgStr.find("LIST") != std::string::npos)
    {
        // Process the LIST request and send the response to the Client
        ListRequest list = ListRequest::parse(msgStr);
        if (list.valid)
        {
            std::string topic_string = my_message_board.List();
            // Send the response to the client
            server.sendToClient(client, topic_string.c_str(), topic_string.length());
        }
    }

    // Check if the incoming request is a COUNT Request
    else if (msgStr.find("COUNT") != std::string::npos)
    {
        // Process the COUNT request and send the response to the Client
        CountRequest count = CountRequest::parse(msgStr);
        if (count.valid)
        {
            unsigned int num_of_post = my_message_board.Count(count.getTopicId());
            // Send the response to the client
            server.sendToClient(client, std::to_string(num_of_post).c_str(), std::to_string(num_of_post).length());
        }
    }

    // Check if the incoming request is a READ Request
    else if (msgStr.find("READ") != std::string::npos)
    {
        // Process the READ request and send the response to the Client
        ReadRequest read = ReadRequest::parse(msgStr);
        if (read.valid)
        {
            std::string post_message = my_message_board.Read(read.getTopicId(), read.getPostId());
            if (post_message.length() != 0)
            {
                // Send the response to the client
                server.sendToClient(client, post_message.c_str(), post_message.length());

            }
            else
            {
                // Send the Error Response to the client
                std::string failed_response("No such post found");
                server.sendToClient(client, failed_response.c_str(), failed_response.length());
            }
        }
    }

    // Check if the incoming EXIT is a READ Request
    else if (msgStr.find("EXIT") != std::string::npos)
    {
        // Process the READ request and send the response to the Client
        ExitRequest exitReq = ExitRequest::parse(msgStr);
        if (exitReq.valid)
        {
            // Send EXIT Response to the client
            server.sendToClient(client, msgStr.c_str(), msgStr.length());
            // close the file descriptor for the client
            close(client->getFileDescriptor());
            // Remove the client from the server
            server.deleteClient(const_cast<Client *>(client));
            std::terminate();
        }
    }
    // Else we have received unknown request
    else
    {
        std::cout << "Invalid Request" << msgStr << std::endl;
        // Send the response to the client
        server.sendToClient(client, msgStr.c_str(), msgStr.length());
    }
}

// observer callback. will be called when client disconnects
void onClientDisconnected(const Client *client) {
    std::cout << "Client: " << client->getIp() << " disconnected: " << client->getInfoMessage() << std::endl;
}

int main() {
    //Start the server on port 12345
    pipe_ret_t startRet = server.start(12345);
    if (startRet.success) {
        std::cout << "Server setup succeeded" << std::endl;
    } else {
        std::cout << "Server setup failed: " << startRet.msg << std::endl;
        return EXIT_FAILURE;
    }

    // configure and register observer 1. This is used to receive and process the incoming message from the
    // any client
    message_board_observer.incoming_packet_func = process_incoming_message;
    message_board_observer.disconnected_func = onClientDisconnected;
    message_board_observer.wantedIp = "";
    server.subscribe(message_board_observer);

    // receive clients
    while (1) {
        // Accept new client and for each new client create new thread
        Client *client = server.acceptClient();
        if (client->isConnected()) {
            std::cout << "Got client with IP: " << client->getIp() << std::endl;
            server.printClients();
        } else {
            std::cout << "Accepting client failed: " << client->getInfoMessage() << std::endl;
        }
    }
}
