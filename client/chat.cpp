#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "chat.h"
#include "exceptions.h"

namespace {

const std::string BLACK = "\u001B[30m";
const std::string GREEN = "\u001B[32m";
const std::string YELLOW = "\u001B[33m";
const std::string BLUE = "\u001B[34m";
const std::string CYAN = "\u001B[36m";
const std::string WHITE = "\u001B[37m";
const std::string GREEN_BG = "\u001B[42m";
const std::string BLUE_BG = "\u001B[44m";
const std::string CYAN_BG = "\u001B[46m";
const std::string WHITE_BG = "\u001B[47m";
const std::string RESET = "\u001B[0m";

} // namespace

/// \brief gets an integer number from the input
static int getInt()
{
    int input = 0;
    while (true) {
        std::cin >> input;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cerr << YELLOW << "Wrong input. Try again \n" << RESET;
        } else {
            std::cin.ignore();
            return input;
        }
    }
}

static void menuPrompt(const std::string& name,
    const std::vector<std::string>& actions, const std::string& color,
    const std::string& background)
{
    int i = 0;
    std::cout << std::endl
              << color << background << " " << name << " " << RESET << "    ";
    for (const auto& a : actions) {
        std::cout << color << background << " " << i++ << " " << RESET << a
                  << "  ";
    }
    std::cout << "\nChoose the action: ";
}

void Chat::showAuthMenu()
{
    std::vector<std::string> actions = { "exit", "sign in", "sign up" };
    while (!is_authorize_) {
        menuPrompt("Main menu", actions, WHITE, GREEN_BG);
        int action = getInt();
        switch (action) {
        case 0:
            exit(0);
        // регистрация нового пользователя
        case 2:
            signUp();
            break;
            // вход по логину ипаролю
        case 1:
            signIn();
            break;
        defualt:
            std::cout << "There is not such in the menu" << std::endl;
        }
    }
}

void Chat::signIn()
{
    std::string username, password;
    std::cout << "Enter the username: ";
    std::cin >> username;
    if (!isUserExisted(username)) {
        throw user_not_existed_error();
    }
    std::cout << "Password: ";
    std::cin >> password;
    if (!isUserExisted(username, password)) {
        throw pass_or_login_not_correct_error();
    }
    is_authorize_ = true;
}
void Chat::selectAction(int action)
{
    switch (action) {
    case 0:
        exit(0);
    case 1:
        bzero(msg_content_, sizeof(msg_content_));
        read(socket_file_descriptor, msg_content_, sizeof(msg_content_));
        std::cout << "Received from " << msg_content_ << std::endl;
        break;

    case 2:
        const int msg_size = 1024;
        std::cout << "Enter the message you want "
                     "to send: "
                  << std::endl;
        bzero(msg_content_, sizeof(msg_content_));
        for (int i = 0; i < users_[current_userid_].getUsername().size(); i++) {
            msg_content_[i] = users_[current_userid_].getUsername()[i];
        }
        char content[msg_size];
        std::cin.getline(content, msg_size, '\n');
        std::cin.getline(content, msg_size, '\n');

        msg_content_[users_[current_userid_].getUsername().length()] = ':';
        msg_content_[users_[current_userid_].getUsername().length() + 1] = ' ';
        strcat(msg_content_, content);

        ssize_t bytes
            = write(socket_file_descriptor, msg_content_, sizeof(msg_content_));
        if (bytes >= 0) {
            std::cout << "Data successfully sent!" << std::endl;
        }
    }
}

void Chat::showMenu()
try {
    const int port = 7777;
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        std::cerr << YELLOW << "Socket creation failed!" << RESET << std::endl;
        exit(1);
    }

    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта
    serveraddress.sin_port = htons(7777);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Установим соединение с сервером
    conn_ = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    if (conn_ == -1) {
        std::cerr << YELLOW << "ERORR: Connection with the server!" << RESET
                  << std::endl;
        exit(1);
    }

    std::cout << "Welcome to Chat!" << std::endl;
    std::fstream user_file
        = std::fstream("user.txt", std::ios::in | std::ios::out);
    if (!user_file) {
        user_file = std::fstream(
            "user.txt", std::ios::in | std::ios::out | std::ios::trunc);
    }
    User obj;
    User objcnrl;
    user_file >> obj;
    while (true) {
        if (obj == objcnrl) {
            break;
        }
        objcnrl = obj;
        user_file >> obj;
    }

    std::cout << std::endl
              << "Number of Reistered users: " << users_.size() << std::endl;

    showAuthMenu();

    while (true) {
        std::vector<std::string> actions
            = { "log out", "display message", "send a message" };

        menuPrompt("User's Menu", actions, WHITE, CYAN_BG);
        int action = getInt();
        selectAction(action);
    }
} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
}

bool Chat::isUserExisted(const std::string& username) const
{
    for (auto user : users_) {
        if (user.getLogin() == username) {
            return true;
        }
    }
    return false;
}

bool Chat::isUserExisted(const std::string& login, const std::string& pass)
{
    int i = 0;
    bool res = false;

    for (auto user : users_) {
        std::string l = user.getLogin();
        std::string p = user.getPassword();
        if (login == l && pass == p) {
            current_userid_ = i;
            res = true;

            std::cout << std::endl
                      << "Hi!, " << user.getUsername() << std::endl
                      << std::endl;
            return res;
        } else {
            i++;
        }
    }
    if (res == false)
        std::cerr << '\n' << "Incorrect password or login!" << std::endl;
    return res;
}

void Chat::signUp()
{
    std::string username, login, password;
    std::cout << "Username: ";
    std::getline(std::cin, username, '\n');
    std::cout << "Login: ";
    bool l = true;
    while (l) {
        std::cin >> login;
        if (isUserExisted(login)) {
            std::cout << "Login is busy!" << std::endl;
            std::cout << "Login: ";
        } else {
            l = false;
        }
    }
    std::cout << "Password: ";
    std::cin >> password;
    User user(username, login, password);
    std::cout << "Welcome to Chat!" << std::endl;
    std::fstream user_file
        = std::fstream("user.txt", std::ios::in | std::ios::app);
    user_file << user;
    users_.push_back(user);

    std::cout << GREEN << "Account was created" << RESET << std::endl;
}

int Chat::getIdUserByName(const std::string& username)
{
    int user_id = 0;
    for (auto user : users_) {
        if (user.getUsername() == username) {
            return user_id;
        } else {
            user_id++;
        }
    }
    return -1;
}
