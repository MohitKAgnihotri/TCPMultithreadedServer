#ifndef __REQUESTPARSER_H
#define __REQUESTPARSER_H

#include <string>
#include <mutex>

class PostRequest
{
private:
    static std::mutex mutex_post_request;
public:
	PostRequest();
	~PostRequest();
	static PostRequest parse(std::string request);
	std::string getTopicId();
	std::string getMessage();
	std::string toString();

	std::string topicId;
	std::string message;
	bool valid;
};

class ReadRequest
{
private:
    static std::mutex mutex_read_request;
public:
	ReadRequest();
	~ReadRequest();
	static ReadRequest parse(std::string request);
	std::string getTopicId();
	int getPostId();
	std::string toString();

	std::string topicId;
	int postId;
	bool valid;
};

class CountRequest
{
private:
    static std::mutex mutex_count_request;
public:
	CountRequest();
	~CountRequest();
	static CountRequest parse(std::string request);
	std::string getTopicId();
	std::string toString();

	std::string topicId;
	bool valid;
};

class ListRequest
{
private:
    static std::mutex mutex_list_request;
public:
	ListRequest();
	~ListRequest();
	static ListRequest parse(std::string request);
	std::string toString();
	bool valid;
};

class ExitRequest
{
private:
    static std::mutex mutex_exit_request;
public:
	ExitRequest();
	~ExitRequest();
	static ExitRequest parse(std::string request);
	std::string toString();
	bool valid;
};

#endif //__REQUESTPARSER_H

