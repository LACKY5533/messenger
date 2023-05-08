#ifndef USERS_H
#define USERS_H
#include <QVector>

using namespace std;

struct User {
    char name[30];
    char password[30];
};

struct Users {
    Users();
    void load();
    void save();
    void add(const char *, const char *);
    User* find(const string& name);
    User* find(const string& name, const string& password);
private:
    QVector<User> users;
    User read_user(ifstream &fst);
    const string filename = "user.dat";
};

#endif // USERS_H
