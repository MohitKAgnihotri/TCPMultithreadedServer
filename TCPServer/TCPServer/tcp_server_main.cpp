
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "RequestParser.h"
#include "message_board.h"
#include "TCPServer.h"

using namespace std;

#define DEFAULT_PORT 12345

// declare the message board
message_board my_message_board;


void Post_Request_Processor(ReceivedSocketData& incoming_connection)
{
    string reply;
	PostRequest post = PostRequest::parse(incoming_connection.request);
	if (post.valid)
	{
		unsigned int postId = my_message_board.Post(post.getTopicId(), post.getMessage());
        incoming_connection.reply = to_string(postId);
	}
}

void List_Request_Processor(ReceivedSocketData& incoming_connection)
{
	// Process the LIST request and send the response to the Client
	ListRequest list = ListRequest::parse(incoming_connection.request);
	if (list.valid)
	{
        incoming_connection.reply = my_message_board.List();
	}
}

void Count_Request_Processor(ReceivedSocketData& incoming_connection)
{
	// Process the COUNT request and send the response to the Client
	CountRequest count = CountRequest::parse(incoming_connection.request);
	if (count.valid)
	{
		unsigned int num_of_post = my_message_board.Count(count.getTopicId());
        incoming_connection.reply = to_string(num_of_post);
	}
}

void Read_Request_Processor(ReceivedSocketData& incoming_connection)
{
	// Process the READ request and send the response to the Client
	ReadRequest read = ReadRequest::parse(incoming_connection.request);
	if (read.valid)
	{
		std::string post_message = my_message_board.Read(read.getTopicId(), read.getPostId());
		if (post_message.length() != 0)
		{
            incoming_connection.reply = post_message;
		}
		else
		{
			// Send the Error Response to the client
            incoming_connection.reply = "No such post found";
		}
	}
}

bool Exit_Request_Processor(ReceivedSocketData& incoming_connection)
{
	// Process the READ request and send the response to the Client
	ExitRequest exitReq = ExitRequest::parse(incoming_connection.request);
	if (exitReq.valid)
	{
        incoming_connection.reply = "Exiting the Client Thread";
        return true;
	}
	return false;
}

// observer callback. will be called for every new message received by clients
// with the requested IP address
void process_incoming_message(TCPServer& server, ReceivedSocketData& incoming_connection)
{

    bool isExitRequested = false;

	while (!isExitRequested)
	{
        server.receiveData(incoming_connection, 1);
		if (incoming_connection.isOperationSuccess)
		{
			// Check if the incoming request is a POST Request
			if (incoming_connection.request.find("POST") != std::string::npos)
			{
				// Process the post request and send the response to the Client
				Post_Request_Processor(incoming_connection);
			}

			// Check if the incoming request is a LIST Request
			else if (incoming_connection.request.find("LIST") != std::string::npos)
			{
				List_Request_Processor(incoming_connection);
			}

			// Check if the incoming request is a COUNT Request
			else if (incoming_connection.request.find("COUNT") != std::string::npos)
			{
				Count_Request_Processor(incoming_connection);
			}

			// Check if the incoming request is a READ Request
			else if (incoming_connection.request.find("READ") != std::string::npos)
			{
				Read_Request_Processor(incoming_connection);
			}

			// Check if the incoming EXIT is a READ Request
			else if (incoming_connection.request.find("EXIT") != std::string::npos)
			{
				if (Exit_Request_Processor(incoming_connection))
				{
					isExitRequested = true;
				}
			}
			// Else we have received unknown request
			else
			{
				std::cout << "Invalid Request" << incoming_connection.request << std::endl;
			}

			incoming_connection.isOperationSuccess = false;
			server.sendReply(incoming_connection);
			if (!incoming_connection.isOperationSuccess)
			{
				isExitRequested = true;
			}
		}
		else
		{
			isExitRequested = true;
		}
	}

    if (isExitRequested)
    {
        server.closeClientSocket(incoming_connection);
		ExitThread(0);
    }
}

int main() {

    TCPServer server(DEFAULT_PORT);

    // receive clients
    while (1) {

        ReceivedSocketData incoming_connection = server.accept();
        thread clientThread = thread(process_incoming_message, server, incoming_connection);
        clientThread.detach();
    }
}
