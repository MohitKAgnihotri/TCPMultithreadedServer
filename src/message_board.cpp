
#include <algorithm>
#include <vector>
#include "topic.h"
#include "message_board.h"

std::mutex message_board::mutex_msgboard;

bool message_board::isTopicExist(std::string topic) {
    return (_message_board_map.count(topic) > 0);
}

void message_board::createNewTopic(std::string topicStr) {
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    std::vector<std::string> post_vector = { };
    _message_board_map.insert({topicStr, post_vector});
}

void message_board::createNewPost(std::string topicStr, std::string message) {
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    _message_board_map[topicStr].push_back(message);
}

unsigned int message_board::Post(std::string usertopic, std::string message) {
    unsigned int postId = 0;
    if (this->isTopicExist(usertopic))
    {
        postId = _message_board_map[usertopic].size();
        createNewPost(usertopic, message);
    }
    else
    {
        postId = 0;
        createNewTopic(usertopic);
        createNewPost(usertopic, message);
    }
    return postId;
}

unsigned int message_board::Count(std::string usertopic)
{
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    if (this->isTopicExist(usertopic)) {
        return _message_board_map[usertopic].size();
    } else {
        return 0;
    }
}

std::string message_board::List() {
    std::string topic_list;
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    for (auto item:_message_board_map) {
        topic_list.append(item.first);
        topic_list.append("#");
    }
    return topic_list;
}

std::string message_board::Read(std::string userTopic, unsigned int i) {
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    std::string post("");
    if (isTopicExist(userTopic))
    {
        if (_message_board_map[userTopic].size() > i) {
            post =_message_board_map[userTopic][i];
        }
    }
    return post;
}
