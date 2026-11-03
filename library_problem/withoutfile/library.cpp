#include "library.h"

int Book::cnt = 0;
int Member::cnt = 0;

vector<Book> books;
vector<Member*> members;
vector<Transaction> transactions;

Book::Book(string t, string a, string p, double pr, int n) {
    book_id = 202600 + (++cnt);
    title = t;
    author = a;
    publisher = p;
    price = pr;
    total_copies = n;

    available.resize(n, true);
}

int Book::getid() {
    return book_id;
}

int Book::issue() {
    for (int i = 0; i < total_copies; i++) {
        if (available[i]) {
            available[i] = false;
            return i + 1;
        }
    }

    return -1;
}

bool Book::return_book(int serial) {
    if (serial < 1 || serial > total_copies)
        return false;

    if (available[serial - 1])
        return false;

    available[serial - 1] = true;
    return true;
}

void Book::display() {
    cout << "\nBook ID: " << book_id
         << "\nTitle: " << title
         << "\nAuthor: " << author
         << "\nPublisher: " << publisher
         << "\nPrice: " << price
         << "\nTotal copies: " << total_copies;

    int count = 0;

    for (int i = 0; i < total_copies; i++)
        if (available[i])
            count++;

    cout << "\nAvailable copies: " << count << "\n";
}


Member::Member(string n, string e, string a) {
    member_id = 2026000 + (++cnt);
    name = n;
    email = e;
    address = a;
    borrowed = 0;
}

Member::~Member() {}

int Member::getid() {
    return member_id;
}

void Member::returned() {
    borrowed--;
}

void Member::display() {
    cout << "\nMember ID: " << member_id
         << "\nName: " << name
         << "\nEmail: " << email
         << "\nAddress: " << address
         << "\nBorrowed books: " << borrowed << "\n";
}


Student::Student(string n, string e, string a)
    : Member(n, e, a) {}

int Student::issue(Book* b) {
    if (borrowed >= 2)
        return -1;

    int serial = b->issue();

    if (serial != -1)
        borrowed++;

    return serial;
}


Faculty::Faculty(string n, string e, string a)
    : Member(n, e, a) {}

int Faculty::issue(Book* b) {
    if (borrowed >= 10)
        return -1;

    int serial = b->issue();

    if (serial != -1)
        borrowed++;

    return serial;
}


Transaction::Transaction(int m, int b, int s) {
    member_id = m;
    book_id = b;
    serial_no = s;

    time_t now = time(0);
    date = ctime(&now);

    returned = false;
}

void Transaction::display() {
    cout << "\nDate: " << date
         << "Member ID: " << member_id
         << "\nBook ID: " << book_id
         << "\nSerial Number: " << serial_no
         << "\nStatus: "
         << (returned ? "Returned" : "Issued")
         << "\n";
}


void add_book() {
    string t, a, p;
    double price;
    int n;

    cout << "\nTitle: ";
    getline(cin, t);

    cout << "Author: ";
    getline(cin, a);

    cout << "Publisher: ";
    getline(cin, p);

    cout << "Price: ";
    cin >> price;

    cout << "Number of copies: ";
    cin >> n;
    cin.ignore();

    if (n <= 0 || price < 0) {
        cout << "Invalid input\n";
        return;
    }

    books.emplace_back(t, a, p, price, n);

    cout << "Book added successfully.\n";
    cout << "Book ID: " << books.back().getid() << "\n";
}


void add_member() {
    string n, e, a;
    int type;

    cout << "\n1. Student\n2. Faculty\n";
    cout << "Enter member type: ";
    cin >> type;
    cin.ignore();

    if (type != 1 && type != 2) {
        cout << "Invalid member type\n";
        return;
    }

    cout << "Name: ";
    getline(cin, n);

    cout << "Email: ";
    getline(cin, e);

    cout << "Address: ";
    getline(cin, a);

    Member* m;

    if (type == 1)
        m = new Student(n, e, a);
    else
        m = new Faculty(n, e, a);

    members.push_back(m);

    cout << "Member registered successfully.\n";
    cout << "Member ID: " << m->getid() << "\n";
}


void issue_book() {
    int mid, bid;

    cout << "\nMember ID: ";
    cin >> mid;

    cout << "Book ID: ";
    cin >> bid;

    Member* m = nullptr;
    Book* b = nullptr;

    for (auto x : members)
        if (x->getid() == mid)
            m = x;

    for (auto &x : books)
        if (x.getid() == bid)
            b = &x;

    if (m == nullptr || b == nullptr) {
        cout << "Invalid member or book ID\n";
        return;
    }

    int serial = m->issue(b);

    if (serial == -1) {
        cout << "Cannot issue book.\n";
        cout << "Borrowing limit reached or no copies available.\n";
        return;
    }

    transactions.emplace_back(mid, bid, serial);

    cout << "Book issued successfully.\n";
    cout << "Serial number: " << serial << "\n";
}


void return_book() {
    int mid, bid, serial;

    cout << "\nMember ID: ";
    cin >> mid;

    cout << "Book ID: ";
    cin >> bid;

    cout << "Serial number: ";
    cin >> serial;

    Member* m = nullptr;
    Book* b = nullptr;

    for (auto x : members)
        if (x->getid() == mid)
            m = x;

    for (auto &x : books)
        if (x.getid() == bid)
            b = &x;

    if (m == nullptr || b == nullptr) {
        cout << "Invalid member or book ID\n";
        return;
    }

    for (auto &t : transactions) {
        if (t.member_id == mid &&
            t.book_id == bid &&
            t.serial_no == serial &&
            !t.returned) {

            if (b->return_book(serial)) {
                t.returned = true;
                m->returned();

                cout << "Book returned successfully.\n";
            }

            return;
        }
    }

    cout << "No matching issue transaction found.\n";
}


void display_books() {
    for (auto &b : books)
        b.display();
}


void display_members() {
    for (auto m : members)
        m->display();
}


void display_transactions() {
    for (auto &t : transactions)
        t.display();
}
