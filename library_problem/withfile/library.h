#ifndef _LIBRARY_H
#define _LIBRARY_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;


class Book {
private:
    static int cnt;

    int book_id;
    string title, author, publisher;
    double price;
    int total_copies;

    vector<int> serial_numbers;
    vector<bool> available;

public:
    static int getcount() {
        return cnt;
    }

    static void setcount(int n) {
        cnt = n;
    }

    Book() {
        book_id = 202600 + (++cnt);
        price = 0;
        total_copies = 0;
    }

    Book(string title, string author, string publisher, double price, int n) {
        book_id = 202600 + (++cnt);

        this->title = title;
        this->author = author;
        this->publisher = publisher;
        this->price = price;
        total_copies = n;

        available.resize(n, true);

        for (int i = 0; i < n; i++)
            serial_numbers.push_back(book_id * 1000 + i + 1);
    }

    // Constructor used by load_data() to restore a book
    Book(int id, string title, string author, string publisher,
         double price, int n, vector<bool> status) {

        book_id = id;

        this->title = title;
        this->author = author;
        this->publisher = publisher;
        this->price = price;
        total_copies = n;

        available = status;

        for (int i = 0; i < n; i++)
            serial_numbers.push_back(book_id * 1000 + i + 1);

        if (book_id - 202600 > cnt)
            cnt = book_id - 202600;
    }

    void retrieve_info() {
        cout << "Enter name of the book : ";
        getline(cin >> ws, title);

        cout << "Enter author : ";
        getline(cin >> ws, author);

        cout << "Enter price of the book : ";
        cin >> price;

        cout << "Enter the publisher of the book : ";
        getline(cin >> ws, publisher);

        cout << "No. of copies present : ";
        cin >> total_copies;

        if (total_copies < 0)
            total_copies = 0;

        serial_numbers.resize(total_copies);
        available.assign(total_copies, true);

        for (int i = 0; i < total_copies; i++)
            serial_numbers[i] = book_id * 1000 + i + 1;
    }

    int issue() {
        for (int i = 0; i < total_copies; i++) {
            if (available[i]) {
                available[i] = false;
                return serial_numbers[i];
            }
        }

        return -1;
    }

    bool return_book(int serial) {
        for (int i = 0; i < total_copies; i++) {
            if (serial_numbers[i] == serial) {
                if (available[i])
                    return false;

                available[i] = true;
                return true;
            }
        }

        return false;
    }

    int getbook_id() {
        return book_id;
    }

    void write_in_file(fstream& f) {
        f << "\nBook ID: " << book_id;
        f << "\nTitle: " << title;
        f << "\nAuthor: " << author;
        f << "\nPublisher: " << publisher;
        f << "\nPrice: " << price;
        f << "\nTotal copies: " << total_copies;
        f << "\nAvailable copies: ";

        for (int i = 0; i < total_copies; i++) {
            if (available[i])
                f << serial_numbers[i] << " ";
            else
                f << -1 << " ";
        }

        f << '\n';
    }

    void display() {
        cout << "\nBook ID: " << book_id;
        cout << "\nTitle: " << title;
        cout << "\nAuthor: " << author;
        cout << "\nPublisher: " << publisher;
        cout << "\nPrice: " << price;
        cout << "\nTotal copies: " << total_copies;
        cout << "\nAvailable copies: ";

        for (int i = 0; i < total_copies; i++) {
            if (available[i])
                cout << serial_numbers[i] << " ";
            else
                cout << -1 << " ";
        }

        cout << "\n";
    }
};


class Member {
protected:
    static int cnt;

    int member_id;
    string name, email, address;
    int borrowed;

public:
    static void setcount(int n) {
        cnt = n;
    }

    // in_member function defined here in order to take input for member addition
    static Member* in_member();

    Member() {
        member_id = 2026000 + (++cnt);
        borrowed = 0;
    }

    Member(string name, string email, string address) {
        member_id = 2026000 + (++cnt);

        this->name = name;
        this->email = email;
        this->address = address;

        borrowed = 0;
    }

    int getmember_id() {
        return member_id;
    }

    void setborrowed(int n) {
        borrowed = n;
    }

    virtual int issue(Book* book) = 0;

    virtual void write_in_file(fstream& f) = 0;

    void returned() {
        if (borrowed > 0)
            borrowed--;
    }

    virtual void display() {
        cout << "\nMember ID: " << member_id;
        cout << "\nName: " << name;
        cout << "\nEmail: " << email;
        cout << "\nAddress: " << address;
        cout << "\nCurrently borrowed: " << borrowed << "\n";
    }

    virtual ~Member() {}
};


class Student : public Member {
public:
    using Member::Member;

    void write_in_file(fstream& f) override {
        f << name << '\n';
        f << email << '\n';
        f << address << '\n';
        f << borrowed << '\n';
        f << "S" << '\n';
    }

    int issue(Book* book) override {
        if (borrowed >= 2)
            return -1;

        int serial = book->issue();

        if (serial != -1)
            borrowed++;

        return serial;
    }
};


class Faculty : public Member {
public:
    using Member::Member;

    int issue(Book* book) override {
        if (borrowed >= 10)
            return -1;

        int serial = book->issue();

        if (serial != -1)
            borrowed++;

        return serial;
    }

    void write_in_file(fstream& f) override {
        f << name << '\n';
        f << email << '\n';
        f << address << '\n';
        f << borrowed << '\n';
        f << "F" << '\n';
    }
};





class Transaction {
public:
    int member_id, book_id, serial_no;
    string date;
    bool returned;

    Transaction() {
        member_id = 0;
        book_id = 0;
        serial_no = 0;
        returned = false;
    }

    Transaction(int member_id, int book_id, int serial_no) {
        this->member_id = member_id;
        this->book_id = book_id;
        this->serial_no = serial_no;

        returned = false;

        time_t now = time(0);
        date = ctime(&now);

        if (!date.empty() && date.back() == '\n')
            date.pop_back();
    }

    // Constructor used by load_data()
    Transaction(int member_id, int book_id, int serial_no,
                bool returned, string date) {

        this->member_id = member_id;
        this->book_id = book_id;
        this->serial_no = serial_no;
        this->returned = returned;
        this->date = date;
    }

    void write_in_file(fstream& f) {
        f << member_id << ' '
          << book_id << ' '
          << serial_no << ' '
          << returned << '\n';

        f << date << '\n';
    }

    void display() {
        cout << "\nDate: " << date;
        cout << "\nMember ID: " << member_id;
        cout << "\nBook ID: " << book_id;
        cout << "\nSerial number: " << serial_no;
        cout << "\nStatus: " << (returned ? "Returned" : "Not returned");
        cout << "\n";
    }
};

#endif
