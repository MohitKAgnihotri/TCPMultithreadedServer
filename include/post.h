//
// Created by 310165137 on 14/01/2021.
//

#ifndef TCP_CLIENT_SERVER_POST_H
#define TCP_CLIENT_SERVER_POST_H

#include <vector>
#include <string>

class post {
    unsigned int Id;
    std::string message;
public:
    post(unsigned int id, const std::string &message) : Id(id), message(message) {}

    unsigned int getId() const {
        return Id;
    }

    const std::string &getMessage() const {
        return message;
    }

    bool operator==(const post &rhs) const {
        return Id == rhs.Id &&
               message == rhs.message;
    }

    bool operator!=(const post &rhs) const {
        return !(rhs == *this);
    }
};


#endif //TCP_CLIENT_SERVER_POST_H
