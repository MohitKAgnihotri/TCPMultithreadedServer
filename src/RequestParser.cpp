#include <regex>

#include "RequestParser.h"

std::mutex PostRequest::mutex_post_request;
std::mutex ReadRequest::mutex_read_request;
std::mutex CountRequest::mutex_count_request;
std::mutex ListRequest::mutex_list_request;
std::mutex ExitRequest::mutex_exit_request;

PostRequest::PostRequest() : valid(0)
{ }

PostRequest::~PostRequest()
{ }

PostRequest PostRequest::parse(const std::string request)
{
    const std::lock_guard<std::mutex> lock(PostRequest::mutex_post_request);
	const std::regex postRegex("^POST(@[^@#]*)#(.+)$");
    std::smatch postMatch;
    PostRequest post;

    if (!std::regex_match(request, postMatch, postRegex, std::regex_constants::match_default)) {
        post.valid = 0;
        return post;
    }

    post.topicId = postMatch[1];
    if (post.topicId.length() >= 140)
        post.topicId.resize(140);
    post.message = postMatch[2];
    if (post.message.length() >= 140)
        post.message.resize(140);
    post.valid = 1;

    return post;
}

std::string PostRequest::getTopicId()
{
	return topicId;
}

std::string PostRequest::getMessage()
{
	return message;
}

std::string PostRequest::toString()
{
    const std::lock_guard<std::mutex> lock(PostRequest::mutex_post_request);
	return std::string("POST") + topicId + std::string("#") + message;
}

ReadRequest::ReadRequest() : valid(0)
{ }

ReadRequest::~ReadRequest()
{ }

ReadRequest ReadRequest::parse(const std::string request)
{
    const std::lock_guard<std::mutex> lock(ReadRequest::mutex_read_request);
	const std::regex readRegex("^READ(@[^@#]*)#([0-9]+)$");
    std::smatch readMatch;
    ReadRequest read;

    if (!std::regex_match(request, readMatch, readRegex, std::regex_constants::match_default)) {
        read.valid = 0;
        return read;
    }

    read.topicId = readMatch[1];
    if (read.topicId.length() >= 140)
        read.topicId.resize(140);
    read.postId = std::stoi(readMatch[2]);
    read.valid = 1;

    return read;
}

std::string ReadRequest::getTopicId()
{
	return topicId;
}

int ReadRequest::getPostId()
{
	return postId;
}

std::string ReadRequest::toString()
{
    const std::lock_guard<std::mutex> lock(ReadRequest::mutex_read_request);
	return std::string("READ") + topicId + std::string("#") + std::to_string(postId);
}

CountRequest::CountRequest() : valid(0)
{ }

CountRequest::~CountRequest()
{ }

CountRequest CountRequest::parse(const std::string request)
{
    const std::lock_guard<std::mutex> lock(CountRequest::mutex_count_request);
	const std::regex countRegex("^COUNT(@[^@#]*)$");
    std::smatch countMatch;
    CountRequest count;

    if (!std::regex_match(request, countMatch, countRegex, std::regex_constants::match_default)) {
        count.valid = 0;
        return count;
    }

    count.topicId = countMatch[1];
    if (count.topicId.length() >= 140)
        count.topicId.resize(140);
    count.valid = 1;

    return count;
}

std::string CountRequest::getTopicId()
{
	return topicId;
}

std::string CountRequest::toString()
{
    const std::lock_guard<std::mutex> lock(CountRequest::mutex_count_request);
	return std::string("COUNT") + topicId;
}

ListRequest::ListRequest() : valid(0)
{ }

ListRequest::~ListRequest()
{ }

ListRequest ListRequest::parse(const std::string request)
{
    const std::lock_guard<std::mutex> lock(ListRequest::mutex_list_request);
	const std::regex listRegex("^LIST$");
	std::smatch listMatch;
	ListRequest list;

	if (!std::regex_match(request, listMatch, listRegex, std::regex_constants::match_default))
	{
		list.valid = 0;
		return list;
	}

	list.valid = 1;

	return list;
}

std::string ListRequest::toString()
{
    const std::lock_guard<std::mutex> lock(ListRequest::mutex_list_request);
	return std::string("LIST");
}

ExitRequest::ExitRequest() : valid(0)
{ }

ExitRequest::~ExitRequest()
{ }

ExitRequest ExitRequest::parse(const std::string request)
{
    const std::lock_guard<std::mutex> lock(ExitRequest::mutex_exit_request);
	const std::regex exitRegex("^EXIT$");
	std::smatch exitMatch;
	ExitRequest exitReq;

	if (!std::regex_match(request, exitMatch, exitRegex, std::regex_constants::match_default))
	{
		exitReq.valid = 0;
		return exitReq;
	}

	exitReq.valid = 1;

	return exitReq;
}

std::string ExitRequest::toString()
{
    const std::lock_guard<std::mutex> lock(ExitRequest::mutex_exit_request);
	return std::string("EXIT");
}
