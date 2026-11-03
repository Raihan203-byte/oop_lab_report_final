#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "library.h"
#include "methods.h"

using namespace std;

/*
    Files:
    members       -> memf
    transactions  -> trnsf
    books         -> bookf
*/

fstream memf, trnsf, bookf;

extern vector<Book> books;
extern vector<Member*> members;
extern vector<Transaction> transactions;

int Book::cnt = 0;
int Member::cnt = 0;


// Extracts the value after the colon in a labelled line.
string get_value(string line) {
    size_t pos = line.find(':');

    if (pos == string::npos)
        return "";

    return line.substr(pos + 1);
}

// in-member function defined here
Member* Member::in_member() {
    string name, email, address;

    cout << "Enter the name : ";
    getline(cin >> ws, name);

    cout << "Enter the email : ";
    getline(cin >> ws, email);

    cout << "Enter the address : ";
    getline(cin >> ws, address);

    cout << "Enter S for student and F for Faculty : ";

    char choice;
    cin >> choice;

    Member* m = nullptr;

    switch (choice) {
        case 'S':
            m = new Student(name, email, address);
            break;

        case 'F':
            m = new Faculty(name, email, address);
            break;

        default:
            cout << "Invalid input\n";
            return nullptr;
    }

    return m;
}


// Creates the files if they don't already exist.
void initialize_files() {
    ofstream temp1("member.lbms", ios::app);
    ofstream temp2("transactions.lbms", ios::app);
    ofstream temp3("books.lbms", ios::app);
}


void load_data() {
    initialize_files();

    Book::setcount(0);
    Member::setcount(0);

    // Load books

    bookf.open("books.lbms", ios::in);

    string line;

    while (getline(bookf, line)) {

        // Skip empty lines
        if (line.empty())
            continue;

        int id = stoi(get_value(line));

        string title, author, publisher;
        double price;
        int copies;

        getline(bookf, line);
        title = get_value(line);

        getline(bookf, line);
        author = get_value(line);

        getline(bookf, line);
        publisher = get_value(line);

        getline(bookf, line);
        price = stod(get_value(line));

        getline(bookf, line);
        copies = stoi(get_value(line));

        getline(bookf, line);

        stringstream ss(get_value(line));

        vector<bool> status(copies);

        for (int i = 0; i < copies; i++) {
            int serial;
            ss >> serial;

            status[i] = (serial != -1);
        }

        books.emplace_back(
            id, title, author, publisher, price, copies, status
        );
    }

    bookf.close();


    // Load members

    memf.open("member.lbms", ios::in);

    while (true) {
        string name, email, address;
        string borrowed_line, type_line;

        if (!getline(memf, name))
            break;

        if (!getline(memf, email))
            break;

        if (!getline(memf, address))
            break;

        if (!getline(memf, borrowed_line))
            break;

        if (!getline(memf, type_line))
            break;

        int borrowed = stoi(borrowed_line);

        Member* m = nullptr;

        if (type_line == "S")
            m = new Student(name, email, address);

        else if (type_line == "F")
            m = new Faculty(name, email, address);

        if (m != nullptr) {
            m->setborrowed(borrowed);
            members.push_back(m);
        }
    }

    memf.close();


    // Load transactions

    trnsf.open("transactions.lbms", ios::in);

    while (true) {
        int mid, bid, serial;
        bool returned;
        string date;

        if (!(trnsf >> mid >> bid >> serial >> returned))
            break;

        getline(trnsf, date);  // Consume the newline
        getline(trnsf, date);

        transactions.emplace_back(
            mid, bid, serial, returned, date
        );
    }

    trnsf.close();
}


void save_data() {
    bookf.open("books.lbms", ios::out | ios::trunc);
    memf.open("member.lbms", ios::out | ios::trunc);
    trnsf.open("transactions.lbms", ios::out | ios::trunc);

    for (Book& b : books)
        b.write_in_file(bookf);

    for (Member* m : members)
        m->write_in_file(memf);

    for (Transaction& t : transactions)
        t.write_in_file(trnsf);

    bookf.close();
    memf.close();
    trnsf.close();
}


void add_book() {
    Book* b = new Book();

    b->retrieve_info();

    books.push_back(*b);

    delete b;

    save_data();

    cout << "\nBook added successfully.\n";
}


void add_member() {
    Member* m = Member::in_member();

    if (m != nullptr) {
        members.push_back(m);

        save_data();

        cout << "\nMember added successfully.\n";
    }
}


void issue_book() {
    int bookid;
    int member_id;

    cout << "Enter book_id : ";
    cin >> bookid;

    cout << "Enter member_id : ";
    cin >> member_id;

    Book* book = nullptr;
    Member* member = nullptr;

    // Find the book
    for (Book& b : books) {
        if (b.getbook_id() == bookid) {
            book = &b;
            break;
        }
    }

    // Find the member
    for (Member* m : members) {
        if (m->getmember_id() == member_id) {
            member = m;
            break;
        }
    }

    if (book == nullptr || member == nullptr) {
        cout << "Invalid book ID or member ID.\n";
        return;
    }

    // The appropriate Student/Faculty issue() is called.
    int serial = member->issue(book);

    if (serial == -1) {
        cout << "Book cannot be issued.\n";
        return;
    }

    transactions.emplace_back(member_id, bookid, serial);

    save_data();

    cout << "Book issued successfully.\n";
    cout << "Serial number: " << serial << '\n';
}


void return_book() {
    int serial;

    cout << "Enter serial number : ";
    cin >> serial;

    for (Transaction& t : transactions) {

        if (t.serial_no == serial && !t.returned) {

            Book* book = nullptr;
            Member* member = nullptr;

            for (Book& b : books) {
                if (b.getbook_id() == t.book_id) {
                    book = &b;
                    break;
                }
            }

            for (Member* m : members) {
                if (m->getmember_id() == t.member_id) {
                    member = m;
                    break;
                }
            }

            if (book == nullptr || member == nullptr) {
                cout << "Invalid transaction data.\n";
                return;
            }

            if (book->return_book(serial)) {
                member->returned();
                t.returned = true;

                save_data();

                cout << "Book returned successfully.\n";
            }

            return;
        }
    }

    cout << "No active transaction found.\n";
}


void display_books() {
    if (books.empty()) {
        cout << "No books available.\n";
        return;
    }

    for (Book& b : books) {
        b.display();
        cout << "-------------------------\n";
    }
}


void display_members() {
    if (members.empty()) {
        cout << "No members available.\n";
        return;
    }

    for (Member* m : members) {
        m->display();
        cout << "-------------------------\n";
    }
}


void display_transactions() {
    if (transactions.empty()) {
        cout << "No transactions available.\n";
        return;
    }

    for (Transaction& t : transactions) {
        t.display();
        cout << "-------------------------\n";
    }
}
