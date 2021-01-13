//
// Created by 310165137 on 14/01/2021.
//

#ifndef TCP_CLIENT_SERVER_MESSAGE_BOARD_H
#define TCP_CLIENT_SERVER_MESSAGE_BOARD_H

#include "post.h"
#include <string>
#include <vector>
#include <map>

class message_board {
private:
    static unsigned int post_ctr;
public:


private:
    std::map<std::string, std::vector<post>> _message_board;
public:


private:
    bool isTopicExist(std::string);
public:
    std::vector <std::string> GetTopicId(void);
    unsigned int InsertPost(std::string, std::string);
    unsigned int GetNumberOfPost(std::string);
    std::string GetPost(std::string, unsigned int);
    const std::map<std::string, std::vector<post>> &getMessageBoard() const;

};

#endif //TCP_CLIENT_SERVER_MESSAGE_BOARD_H
