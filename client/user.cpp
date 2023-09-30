#include "user.h"

std::string User::getUsername() { return username_; }
void User::setUsername(const std::string& name) { username_ = name; }
std::string User::getPassword() { return password_; }
std::string User::getLogin() { return login_; }

// получение наличия сообщений
bool User::isEmpty() { return messages_.size() == 0 ? true : false; }

// получение сообщения
void User::pushMessage(const Message& message) { messages_.push_back(message); }

// чтение сообщений
void User::PrintMessages()
{
    std::cout << std::endl;
    for (auto msg : messages_) {
        msg.PrintM();
    }
    std::cout << std::endl;
}

std::fstream& operator>>(std::fstream& is, User& obj)
{
    is >> obj.username_;
    is >> obj.login_;
    is >> obj.password_;

    return is;
}

std::ostream& operator<<(std::ostream& os, const User& obj)
{
    os << obj.username_;
    os << ' ';
    os << obj.login_;
    os << ' ';
    os << obj.password_;
    os << std::endl;
    return os;
}

void User::sendMessage()
{

    std::fstream user_file2 = std::fstream("msgs.txt", std::ios::in);

    Message obj;
    Message objcnrl;
    user_file2 >> obj;
    while (true) {
        if (obj == objcnrl) {
            break;
        }
        objcnrl = obj;
        if (obj.receiver_ == username_ || obj.receiver_ == "all") {
            messages_.push_back(obj);
        }
        user_file2 >> obj;
    }
}
