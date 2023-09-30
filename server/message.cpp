#include <iostream>
#include <string>

#include "message.h"

void Message::PrintM()
{
    std::cout << "from " << sender_ << ": " << content_ << std::endl;
}

void Message::operator=(Message& other)
{
    sender_ = other.sender_;
    receiver_ = other.receiver_;
    content_ = other.content_;
}
bool Message::operator==(Message& other)
{
    if (sender_ == other.sender_ && receiver_ == other.receiver_
        && content_ == other.content_) {
        return true;
    }
    return false;
}

std::fstream& operator>>(std::fstream& is, Message& obj)
{
    is >> obj.sender_;
    is >> obj.receiver_;
    std::getline(is, obj.content_, '\n');

    return is;
}

std::ostream& operator<<(std::ostream& os, const Message& obj)
{
    os << obj.sender_;
    os << ' ';
    os << obj.receiver_;
    os << ' ';
    os << obj.content_;
    os << std::endl;

    return os;
}
