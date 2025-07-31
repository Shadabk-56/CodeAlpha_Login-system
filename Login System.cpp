#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <conio.h> // For _getch()
using namespace std;

const string adminUser = "Shadabk56";
const string adminPass = "Shadab56@";

string getPasswordInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // Until Enter is pressed
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                cout << "\b \b";
                password.erase(password.size() - 1); // C++98 compatible
            }
        } else {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

bool isValidPassword(const string& password) {
    if (password.length() < 8) return false;

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    for (size_t i = 0; i < password.length(); ++i) {
        char ch = password[i];
        if (isupper(ch)) hasUpper = true;
        else if (islower(ch)) hasLower = true;
        else if (isdigit(ch)) hasDigit = true;
        else hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

bool userExists(const string& username) {
    ifstream infile("users.txt");
    string u, p;
    while (infile >> u >> p) {
        if (u == username)
            return true;
    }
    return false;
}

void registerUser() {
    string username, password;
    cout << "Enter a username: ";
    cin >> username;

    if (userExists(username)) {
        cout << "Username already exists.\n";
        return;
    }

    while (true) {
        cout << "Enter a password: ";
        password = getPasswordInput();
        if (isValidPassword(password)) break;
        else
            cout << "Password must be at least 8 characters, include upper & lowercase, digit, special character.\n";
    }

    ofstream outfile("users.txt", ios::app);
    outfile << username << " " << password << endl;
    cout << "Registration successful.\n";
}

void loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    password = getPasswordInput();

    ifstream infile("users.txt");
    string u, p;
    bool found = false;

    while (infile >> u >> p) {
        if (u == username && p == password) {
            found = true;
            break;
        }
    }

    if (found)
        cout << "Login successful. Welcome, " << username << "!\n";
    else
        cout << "Invalid username or password.\n";
}

void deleteUser() {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    password = getPasswordInput();

    ifstream infile("users.txt");
    ofstream temp("temp.txt");
    string u, p;
    bool deleted = false;

    while (infile >> u >> p) {
        if (u == username && p == password) {
            deleted = true;
            continue;
        }
        temp << u << " " << p << endl;
    }

    infile.close();
    temp.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (deleted)
        cout << "Account deleted successfully.\n";
    else
        cout << "Invalid username or password.\n";
}

void updatePassword() {
    string username, oldPass, newPass;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your current password: ";
    oldPass = getPasswordInput();

    ifstream infile("users.txt");
    ofstream temp("temp.txt");
    string u, p;
    bool updated = false;

    while (infile >> u >> p) {
        if (u == username && p == oldPass) {
            while (true) {
                cout << "Enter new password: ";
                newPass = getPasswordInput();
                if (isValidPassword(newPass)) break;
                else
                    cout << "Password must be at least 8 characters, include upper & lowercase, digit, special character.\n";
            }
            temp << u << " " << newPass << endl;
            updated = true;
        } else {
            temp << u << " " << p << endl;
        }
    }

    infile.close();
    temp.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (updated)
        cout << "Password updated successfully.\n";
    else
        cout << "Incorrect username or current password.\n";
}

void adminDeleteUser() {
    string username;
    cout << "Enter username to delete: ";
    cin >> username;

    ifstream infile("users.txt");
    ofstream temp("temp.txt");
    string u, p;
    bool deleted = false;

    while (infile >> u >> p) {
        if (u == username) {
            deleted = true;
            continue;
        }
        temp << u << " " << p << endl;
    }

    infile.close();
    temp.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (deleted)
        cout << "User deleted successfully.\n";
    else
        cout << "User not found.\n";
}

void viewAllUsers() {
    ifstream infile("users.txt");
    string u, p;
    int count = 0;
    cout << "\n--- Registered Users ---\n";
    while (infile >> u >> p) {
        cout << ++count << ". " << u << endl;
    }
    if (count == 0)
        cout << "No users found.\n";
    cout << "------------------------\n";
}

void adminPanel() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    password = getPasswordInput();

    if (username == adminUser && password == adminPass) {
        int choice;
        do {
            cout << "\n--- Admin Panel ---\n";
            cout << "1. View All Users\n";
            cout << "2. Delete Any User\n";
            cout << "3. Exit Admin Panel\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: viewAllUsers(); break;
                case 2: adminDeleteUser(); break;
                case 3: cout << "Exiting admin panel.\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 3);
    } else {
        cout << "Invalid admin credentials.\n";
    }
}

void userPanel() {
    int choice;
    do {
        cout << "\n--- User Panel ---\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Delete My Account\n";
        cout << "4. Update My Password\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: deleteUser(); break;
            case 4: updatePassword(); break;
            case 5: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

int main() {
    int mainChoice;
    do {
        cout << "\n=== Welcome ===\n";
        cout << "1. User Panel\n";
        cout << "2. Admin Panel\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1: userPanel(); break;
            case 2: adminPanel(); break;
            case 3: cout << "Exiting program. Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (mainChoice != 3);

    return 0;
}

