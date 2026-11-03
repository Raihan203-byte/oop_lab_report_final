#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Book {
    static int cnt;

    int book_id;
    string title, author, publisher;
    double price;
    int total_copies;
    vector<bool> available;

public:
    Book(string t, string a, string p, double pr, int n);

    int getid();
    int issue();
    bool return_book(int serial);
    void display();
};

class Member {
    static int cnt;

protected:
    int member_id;
    string name, email, address;
    int borrowed;

public:
    Member(string n, string e, string a);

    virtual ~Member();

    int getid();
    virtual int issue(Book* b) = 0;
    void returned();
    virtual void display();
};

class Student : public Member {
public:
    Student(string n, string e, string a);

    int issue(Book* b) override;
};

class Faculty : public Member {
public:
    Faculty(string n, string e, string a);

    int issue(Book* b) override;
};

class Transaction {
public:
    int member_id, book_id, serial_no;
    string date;
    bool returned;

    Transaction(int m, int b, int s);

    void display();
};

#endif
