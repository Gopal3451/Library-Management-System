#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// ---------------- BOOK CLASS ----------------
class Book {
public:
    int bookId;
    char title[50];
    int issued; // 0 = no, 1 = yes

    void getBook() {
        cout << "Enter Book ID: ";
        cin >> bookId;
        cin.ignore();
        cout << "Enter Book Title: ";
        cin.getline(title, 50);
        issued = 0;
    }

    void showBook() {
        cout << "\nBook ID: " << bookId;
        cout << "\nTitle: " << title;
        cout << "\nStatus: " << (issued ? "Issued" : "Available") << endl;
    }
};

// ---------------- STUDENT CLASS ----------------
class Student {
public:
    int studentId;
    char name[50];
    int issuedBookId; // -1 if none

    void getStudent() {
        cout << "Enter Student ID: ";
        cin >> studentId;
        cin.ignore();
        cout << "Enter Student Name: ";
        cin.getline(name, 50);
        issuedBookId = -1;
    }

    void showStudent() {
        cout << "\nStudent ID: " << studentId;
        cout << "\nName: " << name;
        cout << "\nIssued Book ID: ";
        if (issuedBookId == -1)
            cout << "None";
        else
            cout << issuedBookId;
        cout << endl;
    }
};

// ---------------- ADMIN LOGIN ----------------
bool adminLogin() {
    string user, pass;
    cout << "\n--- ADMIN LOGIN ---\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    if (user == "admin" && pass == "1234") {
        cout << "\nLogin Successful!\n";
        return true;
    }
    cout << "\nInvalid Login!\n";
    return false;
}

// ---------------- BOOK FUNCTIONS ----------------
void addBook() {
    Book b;
    ofstream out("books.dat", ios::binary | ios::app);
    b.getBook();
    out.write((char*)&b, sizeof(b));
    out.close();
    cout << "\nBook Added Successfully!\n";
}

void displayBooks() {
    Book b;
    ifstream in("books.dat", ios::binary);
    cout << "\n--- BOOK LIST ---\n";
    while (in.read((char*)&b, sizeof(b))) {
        b.showBook();
        cout << "------------------";
    }
    in.close();
}

// ---------------- STUDENT FUNCTIONS ----------------
void addStudent() {
    Student s;
    ofstream out("students.dat", ios::binary | ios::app);
    s.getStudent();
    out.write((char*)&s, sizeof(s));
    out.close();
    cout << "\nStudent Added Successfully!\n";
}

void displayStudents() {
    Student s;
    ifstream in("students.dat", ios::binary);
    cout << "\n--- STUDENT LIST ---\n";
    while (in.read((char*)&s, sizeof(s))) {
        s.showStudent();
        cout << "-------------------";
    }
    in.close();
}

// ---------------- ISSUE BOOK ----------------
void issueBook() {
    int sid, bid;
    Book b;
    Student s;

    cout << "Enter Student ID: ";
    cin >> sid;
    cout << "Enter Book ID: ";
    cin >> bid;

    fstream bf("books.dat", ios::binary | ios::in | ios::out);
    fstream sf("students.dat", ios::binary | ios::in | ios::out);

    // Check book
    while (bf.read((char*)&b, sizeof(b))) {
        if (b.bookId == bid && b.issued == 0) {
            b.issued = 1;
            bf.seekp(-sizeof(b), ios::cur);
            bf.write((char*)&b, sizeof(b));
            break;
        }
    }

    // Assign book to student
    while (sf.read((char*)&s, sizeof(s))) {
        if (s.studentId == sid && s.issuedBookId == -1) {
            s.issuedBookId = bid;
            sf.seekp(-sizeof(s), ios::cur);
            sf.write((char*)&s, sizeof(s));
            cout << "\nBook Issued Successfully!\n";
            bf.close();
            sf.close();
            return;
        }
    }

    cout << "\nIssue Failed!\n";
    bf.close();
    sf.close();
}

// ---------------- RETURN BOOK ----------------
void returnBook() {
    int sid;
    Student s;
    Book b;

    cout << "Enter Student ID: ";
    cin >> sid;

    fstream sf("students.dat", ios::binary | ios::in | ios::out);
    fstream bf("books.dat", ios::binary | ios::in | ios::out);

    int bid = -1;

    while (sf.read((char*)&s, sizeof(s))) {
        if (s.studentId == sid && s.issuedBookId != -1) {
            bid = s.issuedBookId;
            s.issuedBookId = -1;
            sf.seekp(-sizeof(s), ios::cur);
            sf.write((char*)&s, sizeof(s));
            break;
        }
    }

    while (bf.read((char*)&b, sizeof(b))) {
        if (b.bookId == bid) {
            b.issued = 0;
            bf.seekp(-sizeof(b), ios::cur);
            bf.write((char*)&b, sizeof(b));
            break;
        }
    }

    cout << "\nBook Returned Successfully!\n";
    sf.close();
    bf.close();
}

// ---------------- MAIN ----------------
int main() {
    if (!adminLogin())
        return 0;

    int choice;
    do {
        cout << "\n===== LIBRARY MANAGEMENT =====";
        cout << "\n1. Add Book";
        cout << "\n2. View Books";
        cout << "\n3. Add Student";
        cout << "\n4. View Students";
        cout << "\n5. Issue Book";
        cout << "\n6. Return Book";
        cout << "\n7. Exit";
        cout << "\nEnter Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addBook(); break;
        case 2: displayBooks(); break;
        case 3: addStudent(); break;
        case 4: displayStudents(); break;
        case 5: issueBook(); break;
        case 6: returnBook(); break;
        case 7: cout << "\nThank You!\n"; break;
        default: cout << "\nInvalid Choice!\n";
        }
    } while (choice != 7);

    return 0;
}
