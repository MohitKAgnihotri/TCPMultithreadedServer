
#include <iostream>
#include <string>
#include <vector>

#include "RequestParser.h"
#include "post.h"
#include "message_board.h"

#include <iostream>
#include <signal.h>
#include <string>

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
void process_incoming_message(const Client &client, const char *msg, size_t size) {
    std::string msgStr = msg;
    // print the message content
    // if client sent the string "quit", close server
    // else if client sent "print" print the server clients
    // else just print the client message
    if (msgStr.find("POST") != std::string::npos) {
        std::cout << "Post Request Received" << std::endl;
        PostRequest post = PostRequest::parse(msgStr);
        if (post.valid) {
            std::cout << "Post request: " << post.toString() << std::endl;
            std::cout << "Post topic: " << post.getTopicId() << std::endl;
            std::cout << "Post message: " << post.getMessage() << std::endl;
            unsigned int postId = my_message_board.Post(post.getTopicId(), post.getMessage());
            server.sendToClient(client, std::to_string(postId).c_str(), 1);
        }
    } else if (msgStr.find("LIST") != std::string::npos) {
        std::cout << "List Request Received" << std::endl;
        ListRequest list = ListRequest::parse(msgStr);
        if (list.valid) {
            std::cout << "List request: " << list.toString() << std::endl;
            std::string topic_string = my_message_board.List();
            server.sendToClient(client, topic_string.c_str(), topic_string.length());
        }
    } else if (msgStr.find("COUNT") != std::string::npos) {
        std::cout << "COUNT Request Received" << std::endl;
        CountRequest count = CountRequest::parse(msgStr);
        if (count.valid) {
            std::cout << "Count request: " << count.toString() << std::endl;
            std::cout << "Count topic: " << count.getTopicId() << std::endl;
            unsigned int num_of_post = my_message_board.Count(count.getTopicId());
            server.sendToClient(client, std::to_string(num_of_post).c_str(), 1);
        }
    } else if (msgStr.find("READ") != std::string::npos) {
        std::cout << "READ Request Received" << std::endl;
        ReadRequest read = ReadRequest::parse(msgStr);
        if (read.valid) {
            std::cout << "Read request" << read.toString() << std::endl;
            std::cout << "Read topic: " << read.getTopicId() << std::endl;
            std::cout << "Read post id: " << read.getPostId() << std::endl;
            std::string post_message = my_message_board.Read(read.getTopicId(), read.getPostId());
            server.sendToClient(client, post_message.c_str(), post_message.length());
        }
    } else if (msgStr.find("EXIT") != std::string::npos) {
        std::cout << "Exit Request Received" << std::endl;
        ExitRequest exitReq = ExitRequest::parse(msgStr);
        if (exitReq.valid) {
            std::cout << "Exit request: " << exitReq.toString() << std::endl;
            pipe_ret_t ret = server.finish();
            if (ret.success) {
                kill(getppid(), SIGUSR1);
            }
        }
    } else {
        std::cout << "Invalid Request" << msgStr << std::endl;
    }
}

// observer callback. will be called when client disconnects
void onClientDisconnected(const Client &client) {
    std::cout << "Client: " << client.getIp() << " disconnected: " << client.getInfoMessage() << std::endl;
}

void sig_exit(int s) {
    std::cout << "Closing Server..." << std::endl;
    exit(0);
}

int main() {
    //register to SIGINT to close client when user press ctrl+c
    signal(SIGUSR1, sig_exit);

    // start server on port 65123
    pipe_ret_t startRet = server.start(12345);
    if (startRet.success) {
        std::cout << "Server setup succeeded" << std::endl;
    } else {
        std::cout << "Server setup failed: " << startRet.msg << std::endl;
        return EXIT_FAILURE;
    }

    // configure and register observer1
    message_board_observer.incoming_packet_func = process_incoming_message;
    message_board_observer.disconnected_func = onClientDisconnected;
    message_board_observer.wantedIp = "";
    server.subscribe(message_board_observer);

    // receive clients
    while (1) {
        Client client = server.acceptClient();
        if (client.isConnected()) {
            std::cout << "Got client with IP: " << client.getIp() << std::endl;
            server.printClients();
        } else {
            std::cout << "Accepting client failed: " << client.getInfoMessage() << std::endl;
        }
    }
}
