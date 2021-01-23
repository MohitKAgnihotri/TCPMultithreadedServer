
#include <algorithm>
#include <vector>
#include <mutex>
#include "message_board.h"

std::mutex message_board::mutex_msgboard;

/*
 * Check if the key exist in the map, if yes, then the topic exist else not
 * */
bool message_board::isTopicExist(const std::string &topic) {
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    return (_message_board_map.count(topic) > 0);
}

/*
 * Create a new topic for posting.
 * make sure to ensure exclusive access to avoid race conditions.
 * */
void message_board::createNewTopic(const std::string &topicStr) {
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    std::vector<std::string> post_vector = { };
    _message_board_map.insert({topicStr, post_vector});
}


/*
 * Create a new post for the existing topic
 * make sure to ensure exclusive access to avoid race conditions.
 * */
void message_board::createNewPost(const std::string &topicStr, const std::string &message) {
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    _message_board_map[topicStr].push_back(message);
}


/*
 * User function exposed to the user. This adds post to the existing or creates a new topic.
 * */
unsigned int message_board::Post(const std::string &usertopic, const std::string &message) {
    unsigned int postId;
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

/*
 * User facing function
 * This is used to get the count of post for a given topic
 * */
unsigned int message_board::Count(const std::string &usertopic)
{
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    if (this->isTopicExist(usertopic)) {
        return _message_board_map[usertopic].size();
    } else {
        return 0;
    }
}

/*
 * User facing function
 * This is used to get the # seperated list of the topics.
 * */
std::string message_board::List() {
    std::string topic_list;
    const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
    for (const auto& item:_message_board_map) {
        topic_list.append(item.first);
        topic_list.append("#");
    }
    return topic_list;
}

/*
 * User facing function
 * This is used to retrieve a specific post for a given topic
 * */
std::string message_board::Read(const std::string &userTopic, unsigned int i) {
    
    std::string post;
    if (isTopicExist(userTopic))
    {
        const std::lock_guard<std::mutex> lock(message_board::mutex_msgboard);
        if (_message_board_map[userTopic].size() > i) {
            post =_message_board_map[userTopic][i];
        }
    }
    return post;
}
