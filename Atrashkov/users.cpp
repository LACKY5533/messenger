#include "users.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Users::Users()
{

}

User Users::read_user(ifstream &ifst) {
    User user;
    ifst.read((char*)&user, sizeof(User));
    return user;
}

void Users::load() //Функция, которая загружает информацию о логинах и паролях пользователей из файла в вектор
{
    ifstream ifst(filename, ios::binary);
        if(!ifst) {
            ifst.open(filename, ios::out);
            ifst.close();
            ifst.open(filename, ios::binary);
        }
        ifst.seekg(0, ios::end);
        int n = ifst.tellg() / sizeof(User);
        ifst.seekg(0, ios::beg);
        for (int i = 0; i < n; ++i) {
            users.push_back(read_user(ifst));
        }
}

void Users::add(const char* name, const char* password) { //Функция, которая заносит данные пользователя в вектор
    User user;
    strncpy(user.name, name, 30);
    strncpy(user.password, password, 30);
    users.push_back(user);
}

void Users::save() { //Функция, которая сохраняет данные в файл
    ofstream ofst;
    ofst.open(filename, ios::out);
    for (auto& user : users) {
        ofst.write((char*)&user, sizeof(User));
    }
}

User* Users::find(const string& name) { //Функция для поиска логина в файле
    for (auto& user : users) {
        if (name == user.name)
            return &user;
    }
    return nullptr;
}

User* Users::find(const string& name, const string& password) { //Функция для поиска логина и пароля в файле
    for (auto& user : users) {
        if (name == user.name && password == user.password)
            return &user;
    }
    return nullptr;
}
