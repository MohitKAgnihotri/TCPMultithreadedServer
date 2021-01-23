
#ifndef TCP_CLIENT_SERVER_MESSAGE_BOARD_H
#define TCP_CLIENT_SERVER_MESSAGE_BOARD_H

#include <unordered_map>
#include <mutex>

class message_board {
    std::unordered_map<std::string, std::vector<std::string>> _message_board_map;
    static std::mutex mutex_msgboard;
public:

    std::string Read(const std::string& userTopic, unsigned int i);

    std::string List();

    unsigned int Count(const std::string& usertopic);

    unsigned int Post(const std::string& usertopic, const std::string& message);

    void createNewPost(const std::string& topicStr, const std::string& message);

    void createNewTopic(const std::string& topicStr);

    bool isTopicExist(const std::string& topic);
};


#endif //TCP_CLIENT_SERVER_MESSAGE_BOARD_H
