#include "library.h"

extern vector<Book> books;
extern vector<Member*> members;
extern vector<Transaction> transactions;

void add_book();
void add_member();
void issue_book();
void return_book();

void display_books();
void display_members();
void display_transactions();

int main() {
    int choice;

    while (true) {
        cout << "\n========== LIBRARY MANAGEMENT ==========\n";

        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Display Books\n";
        cout << "6. Display Members\n";
        cout << "7. Display Transactions\n";
        cout << "8. Exit\n";

        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                add_book();
                break;

            case 2:
                add_member();
                break;

            case 3:
                issue_book();
                break;

            case 4:
                return_book();
                break;

            case 5:
                display_books();
                break;

            case 6:
                display_members();
                break;

            case 7:
                display_transactions();
                break;

            case 8:
                for (auto m : members)
                    delete m;

                return 0;

            default:
                cout << "Invalid choice\n";
        }
    }
}
