#include "library.h"
#include "methods.h"
#include <iostream>

using namespace std;

vector<Book> books;
vector<Member*> members;
vector<Transaction> transactions;


int main() {
    load_data();

    int choice;

    while (true) {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Display Books\n";
        cout << "6. Display Members\n";
        cout << "7. Display Transactions\n";
        cout << "0. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

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

            case 0:
                save_data();

                for (Member* m : members)
                    delete m;

                members.clear();

                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    }
}
