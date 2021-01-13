//
// Created by 310165137 on 14/01/2021.
//

#include "message_board.h"

std::vector <std::string> message_board::GetTopicId(void) {
    std::vector<std::string> topics;
    for(auto item : getMessageBoard())
    {
        topics.push_back(item.first);
    }
    return topics;
}

unsigned int message_board::InsertPost(std::string topicId, std::string message)
{
    if (isTopicExist(topicId))
    {
        auto posts = getMessageBoard().find(topicId);
    }
    else
    {
        getMessageBoard().insert(std::pair<std::string,std::vector<std::string>>(topicId,std::vector<std::string>{message}));
    }
    return 34;
}

unsigned int message_board::GetNumberOfPost(std::string topicId) {
    if (isTopicExist(topicId))
    {
        auto posts = getMessageBoard().find(topicId);
    }
    return 4;
}

std::string message_board::GetPost(std::string topicId, unsigned int postId) {
    std::string post = "";
    if (isTopicExist(topicId))
    {
        auto posts = getMessageBoard().find(topicId);
    }
    return post;
}

const std::map<std::string, std::vector<post>> &message_board::getMessageBoard() const {
    return _message_board;
}

bool message_board::isTopicExist(std::string topic) {
    auto topicIterator = getMessageBoard().find(topic);
    if (topicIterator->first.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}
