//
// Created by 310165137 on 13/01/2021.
//

#ifndef TCP_CLIENT_SERVER_POST_H
#define TCP_CLIENT_SERVER_POST_H


#include <string>
#include <vector>
#include <map>


class post
{
private:
    unsigned int _postId;
    std::string _message;
public:
    post(unsigned int postId, std::string message)
    {
        _postId = postId;
        _message = message;
    }
};


#endif //TCP_CLIENT_SERVER_POST_H
