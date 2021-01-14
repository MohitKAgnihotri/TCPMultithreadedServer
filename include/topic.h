//
// Created by 310165137 on 14/01/2021.
//

#ifndef TCP_CLIENT_SERVER_TOPIC_H
#define TCP_CLIENT_SERVER_TOPIC_H

#include <utility>

#include "post.h"

class topic {
    std::string _topic;
    std::vector<post> posts;
public:
    topic(std::string topic) : _topic(std::move(topic)) {}

    const std::string &getTopic() const {
        return _topic;
    }

    const std::vector<post> &getPosts() const {
        return posts;
    }

    unsigned int insertPost(unsigned int, const std::string &);

    unsigned int getNumberofPost();

    std::string getPost(unsigned int postId);

    bool operator==(const topic &rhs) const {
        return _topic == rhs._topic &&
               posts == rhs.posts;
    }

    bool operator!=(const topic &rhs) const {
        return !(rhs == *this);
    }
};


#endif //TCP_CLIENT_SERVER_TOPIC_H
