#include <iostream>
#include <fstream>
#include <string>

using namespace std;


// ======================================================
// BOOK
// ======================================================

struct Book {
    int id;
    string title;
    string author;
    int year;
    bool available;
};


// ======================================================
// NODE
// Used by Linked List
// ======================================================

struct Node {
    Book data;
    Node* next;

    Node(Book b) {
        data = b;
        next = nullptr;
    }
};


// ======================================================
// BORROW REQUEST
// Used by Queue
// ======================================================

struct BorrowRequest {
    int bookId;
    string borrowerName;
};


// ======================================================
// BOOK LIST
// Linked List + Merge Sort + File Handling
// ======================================================

class BookList {

private:

    Node* head;


    // ---------------- MERGE ----------------

    Node* merge(Node* a, Node* b) {

        if (a == nullptr)
            return b;

        if (b == nullptr)
            return a;

        Node* result;

        if (a->data.year <= b->data.year) {

            result = a;
            result->next = merge(a->next, b);

        }
        else {

            result = b;
            result->next = merge(a, b->next);
        }

        return result;
    }


    // ---------------- SPLIT ----------------

    void split(Node* source, Node*& left, Node*& right) {

        Node* slow = source;
        Node* fast = source->next;

        while (fast != nullptr) {

            fast = fast->next;

            if (fast != nullptr) {

                slow = slow->next;
                fast = fast->next;
            }
        }

        left = source;
        right = slow->next;
        slow->next = nullptr;
    }


    // ---------------- MERGE SORT ----------------

    Node* mergeSort(Node* node) {

        if (node == nullptr || node->next == nullptr)
            return node;

        Node* left;
        Node* right;

        split(node, left, right);

        left = mergeSort(left);
        right = mergeSort(right);

        return merge(left, right);
    }


public:

    // ---------------- CONSTRUCTOR ----------------

    BookList() {
        head = nullptr;
    }


    // ---------------- ADD BOOK ----------------

    void addBook(Book book) {

        Node* newNode = new Node(book);

        if (head == nullptr) {

            head = newNode;
            return;
        }

        Node* temp = head;

        while (temp->next != nullptr)
            temp = temp->next;

        temp->next = newNode;
    }


    // ---------------- SEARCH BOOK ----------------

    Node* searchBook(int id) {

        Node* temp = head;

        while (temp != nullptr) {

            if (temp->data.id == id)
                return temp;

            temp = temp->next;
        }

        return nullptr;
    }


    // ---------------- DISPLAY BOOKS ----------------

    void displayBooks() {

        if (head == nullptr) {

            cout << "\nLibrary is empty.\n";
            return;
        }

        Node* temp = head;

        cout << "\n========== BOOKS ==========\n";

        while (temp != nullptr) {

            cout << "ID: " << temp->data.id << endl;
            cout << "Title: " << temp->data.title << endl;
            cout << "Author: " << temp->data.author << endl;
            cout << "Year: " << temp->data.year << endl;

            if (temp->data.available)
                cout << "Status: Available\n";
            else
                cout << "Status: Borrowed\n";

            cout << "---------------------------\n";

            temp = temp->next;
        }
    }


    // ---------------- UPDATE BOOK ----------------

    void updateBook(int id) {

        Node* book = searchBook(id);

        if (book == nullptr) {

            cout << "\nBook not found.\n";
            return;
        }

        cout << "New title: ";
        getline(cin >> ws, book->data.title);

        cout << "New author: ";
        getline(cin >> ws, book->data.author);

        cout << "New year: ";
        cin >> book->data.year;

        cout << "\nBook updated successfully.\n";
    }


    // ---------------- DELETE BOOK ----------------

    void deleteBook(int id) {

        if (head == nullptr) {

            cout << "\nLibrary is empty.\n";
            return;
        }


        // Delete first node

        if (head->data.id == id) {

            Node* temp = head;

            head = head->next;

            delete temp;

            cout << "\nBook deleted successfully.\n";
            return;
        }


        // Delete middle or last node

        Node* temp = head;

        while (temp->next != nullptr) {

            if (temp->next->data.id == id) {

                Node* remove = temp->next;

                temp->next = remove->next;

                delete remove;

                cout << "\nBook deleted successfully.\n";
                return;
            }

            temp = temp->next;
        }

        cout << "\nBook not found.\n";
    }


    // ---------------- SORT BOOKS ----------------

    void sortByYear() {

        head = mergeSort(head);

        cout << "\nBooks sorted by publication year.\n";
    }


    // ---------------- LOAD FILE ----------------

    void loadFromFile() {

        ifstream file("books.txt");

        if (!file) {

            cout << "\nbooks.txt not found.\n";
            return;
        }


        while (true) {

            Book book;

            if (!(file >> book.id))
                break;

            file.ignore();

            getline(file, book.title);
            getline(file, book.author);

            file >> book.year;
            file >> book.available;

            addBook(book);
        }

        file.close();
    }


    // ---------------- SAVE FILE ----------------

    void saveToFile() {

        ofstream file("books.txt");

        Node* temp = head;

        while (temp != nullptr) {

            file << temp->data.id << endl;
            file << temp->data.title << endl;
            file << temp->data.author << endl;
            file << temp->data.year << endl;
            file << temp->data.available << endl;

            temp = temp->next;
        }

        file.close();
    }
};


// ======================================================
// BORROW QUEUE
// FIFO
// ======================================================

class BorrowQueue {

private:

    BorrowRequest requests[100];

    int front;
    int rear;


public:

    // ---------------- CONSTRUCTOR ----------------

    BorrowQueue() {

        front = 0;
        rear = 0;
    }


    // ---------------- ENQUEUE ----------------

    void enqueue(BorrowRequest request) {

        if (rear == 100) {

            cout << "\nQueue is full.\n";
            return;
        }

        requests[rear] = request;

        rear++;
    }


    // ---------------- DEQUEUE ----------------

    bool dequeue(BorrowRequest& request) {

        if (front == rear)
            return false;

        request = requests[front];

        front++;

        return true;
    }


    // ---------------- DISPLAY QUEUE ----------------

    void displayQueue() {

        if (front == rear) {

            cout << "\nBorrow queue is empty.\n";
            return;
        }

        cout << "\n======= BORROW QUEUE =======\n";

        for (int i = front; i < rear; i++) {

            cout << "Book ID: "
                 << requests[i].bookId << endl;

            cout << "Borrower: "
                 << requests[i].borrowerName << endl;

            cout << "----------------------------\n";
        }
    }
};


// ======================================================
// RETURN STACK
// LIFO
// ======================================================

class ReturnStack {

private:

    Book books[100];

    int top;


public:

    // ---------------- CONSTRUCTOR ----------------

    ReturnStack() {

        top = -1;
    }


    // ---------------- PUSH ----------------

    void push(Book book) {

        if (top == 99) {

            cout << "\nReturn stack is full.\n";
            return;
        }

        top++;

        books[top] = book;
    }


    // ---------------- POP ----------------

    void pop() {

        if (top == -1) {

            cout << "\nReturn history is empty.\n";
            return;
        }

        cout << "\nRemoving: "
             << books[top].title << endl;

        top--;
    }


    // ---------------- DISPLAY ----------------

    void displayHistory() {

        if (top == -1) {

            cout << "\nReturn history is empty.\n";
            return;
        }

        cout << "\n======= RETURN HISTORY =======\n";

        for (int i = top; i >= 0; i--) {

            cout << "Book ID: "
                 << books[i].id << endl;

            cout << "Title: "
                 << books[i].title << endl;

            cout << "------------------------------\n";
        }
    }
};


// ======================================================
// MAIN
// ======================================================

int main() {

    BookList library;

    BorrowQueue borrowQueue;

    ReturnStack returnStack;


    // Load books from file

    library.loadFromFile();


    int choice;


    do {

        cout << "\n\n";
        cout << "=================================\n";
        cout << "     LIBRARY MANAGEMENT SYSTEM\n";
        cout << "=================================\n";

        cout << "1. Add Book\n";
        cout << "2. Display Books\n";
        cout << "3. Search Book\n";
        cout << "4. Update Book\n";
        cout << "5. Delete Book\n";
        cout << "6. Borrow Book\n";
        cout << "7. Display Borrow Queue\n";
        cout << "8. Return Book\n";
        cout << "9. Display Return History\n";
        cout << "10. Sort Books by Year\n";
        cout << "0. Exit\n";

        cout << "=================================\n";

        cout << "Enter choice: ";
        cin >> choice;


        // ==================================================
        // 1. ADD BOOK
        // ==================================================

        if (choice == 1) {

            Book book;

            cout << "\nBook ID: ";
            cin >> book.id;


            // Check duplicate ID

            if (library.searchBook(book.id) != nullptr) {

                cout << "\nID already exists.\n";
                continue;
            }


            cout << "Title: ";
            getline(cin >> ws, book.title);

            cout << "Author: ";
            getline(cin >> ws, book.author);

            cout << "Year: ";
            cin >> book.year;

            book.available = true;


            library.addBook(book);

            library.saveToFile();

            cout << "\nBook added successfully.\n";
        }


        // ==================================================
        // 2. DISPLAY BOOKS
        // ==================================================

        else if (choice == 2) {

            library.displayBooks();
        }


        // ==================================================
        // 3. SEARCH BOOK
        // ==================================================

        else if (choice == 3) {

            int id;

            cout << "\nEnter book ID: ";
            cin >> id;


            Node* book = library.searchBook(id);


            if (book == nullptr) {

                cout << "\nBook not found.\n";
            }
            else {

                cout << "\nBook found!\n";

                cout << "ID: "
                     << book->data.id << endl;

                cout << "Title: "
                     << book->data.title << endl;

                cout << "Author: "
                     << book->data.author << endl;

                cout << "Year: "
                     << book->data.year << endl;


                if (book->data.available)
                    cout << "Status: Available\n";
                else
                    cout << "Status: Borrowed\n";
            }
        }


        // ==================================================
        // 4. UPDATE BOOK
        // ==================================================

        else if (choice == 4) {

            int id;

            cout << "\nEnter book ID: ";
            cin >> id;

            library.updateBook(id);

            library.saveToFile();
        }


        // ==================================================
        // 5. DELETE BOOK
        // ==================================================

        else if (choice == 5) {

            int id;

            cout << "\nEnter book ID: ";
            cin >> id;

            library.deleteBook(id);

            library.saveToFile();
        }


        // ==================================================
        // 6. BORROW BOOK
        // ==================================================

        else if (choice == 6) {

            int id;
            string name;


            cout << "\nEnter book ID: ";
            cin >> id;


            Node* book = library.searchBook(id);


            // Book doesn't exist

            if (book == nullptr) {

                cout << "\nBook not found.\n";
                continue;
            }


            // Book already borrowed

            if (!book->data.available) {

                cout << "\nBook is already borrowed.\n";
                continue;
            }


            cout << "Borrower name: ";
            getline(cin >> ws, name);


            BorrowRequest request;

            request.bookId = id;
            request.borrowerName = name;


            // Add borrower to Queue

            borrowQueue.enqueue(request);


            // Book becomes unavailable

            book->data.available = false;


            library.saveToFile();


            cout << "\nBook borrowed successfully.\n";
        }


        // ==================================================
        // 7. DISPLAY BORROW QUEUE
        // ==================================================

        else if (choice == 7) {

            borrowQueue.displayQueue();
        }


        // ==================================================
        // 8. RETURN BOOK
        // ==================================================

        else if (choice == 8) {

            int id;

            cout << "\nEnter book ID: ";
            cin >> id;


            Node* book = library.searchBook(id);


            if (book == nullptr) {

                cout << "\nBook not found.\n";
                continue;
            }


            if (book->data.available) {

                cout << "\nThis book is not borrowed.\n";
                continue;
            }


            // Make book available

            book->data.available = true;


            // Add returned book to Stack

            returnStack.push(book->data);


            library.saveToFile();


            cout << "\nBook returned successfully.\n";
        }


        // ==================================================
        // 9. DISPLAY RETURN HISTORY
        // ==================================================

        else if (choice == 9) {

            returnStack.displayHistory();
        }


        // ==================================================
        // 10. SORT BOOKS
        // ==================================================

        else if (choice == 10) {

            library.sortByYear();
        }


        // ==================================================
        // 0. EXIT
        // ==================================================

        else if (choice == 0) {

            library.saveToFile();

            cout << "\nData saved.\n";
            cout << "Goodbye!\n";
        }


        else {

            cout << "\nInvalid choice.\n";
        }


    } while (choice != 0);


    return 0;
}