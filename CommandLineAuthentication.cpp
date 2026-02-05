#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <functional>

using namespace std;

/* =========================
   User Class
   ========================= */
class User {
private:
    string username;
    size_t passwordHash;
    string role; // "user" or "admin"

public:
    User(string u = "", size_t h = 0, string r = "user") {
        username = u;
        passwordHash = h;
        role = r;
    }

    string getUsername() const { return username; }
    string getRole() const { return role; }

    bool verifyPassword(const string& password) const {
        return hash<string>{}(password) == passwordHash;
    }

    string toFileString() const {
        return username + "|" + to_string(passwordHash) + "|" + role;
    }

    static User fromFileString(const string& line) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        string u = line.substr(0, p1);
        size_t h = stoull(line.substr(p1 + 1, p2 - p1 - 1));
        string r = line.substr(p2 + 1);

        return User(u, h, r);
    }
};

/* =========================
   Auth Manager Class
   ========================= */
class AuthManager {
private:
    vector<User> users;

public:
    void loadUsers() {
        users.clear();
        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            users.push_back(User::fromFileString(line));
        }
    }

    void saveUsers() {
        ofstream file("users.txt");
        for (const auto& u : users) {
            file << u.toFileString() << endl;
        }
    }

    bool registerUser(const string& username, const string& password, const string& role) {
        for (const auto& u : users) {
            if (u.getUsername() == username)
                return false;
        }
        size_t hashVal = hash<string>{}(password);
        users.push_back(User(username, hashVal, role));
        saveUsers();
        return true;
    }

    User* login(const string& username, const string& password) {
        for (auto& u : users) {
            if (u.getUsername() == username && u.verifyPassword(password))
                return &u;
        }
        return nullptr;
    }

    void deleteUser(const string& username) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->getUsername() == username) {
                users.erase(it);
                saveUsers();
                return;
            }
        }
    }

    const vector<User>& getAllUsers() const {
        return users;
    }
};

/* =========================
   Main Program
   ========================= */
int main() {
    AuthManager auth;
    auth.loadUsers();

    User* currentUser = nullptr;
    int choice;

    do {
        if (!currentUser) {
            cout << "\n--- AUTH SYSTEM ---\n";
            cout << "1. Register\n2. Login\n3. Exit\n";
            cout << "Choice: ";
            cin >> choice;

            if (choice == 1) {
                string u, p, r;
                cout << "Username: ";
                cin >> u;
                cout << "Password: ";
                cin >> p;
                cout << "Role (user/admin): ";
                cin >> r;

                if (auth.registerUser(u, p, r))
                    cout << "User registered.\n";
                else
                    cout << "Username already exists.\n";
            }
            else if (choice == 2) {
                string u, p;
                cout << "Username: ";
                cin >> u;
                cout << "Password: ";
                cin >> p;

                currentUser = auth.login(u, p);
                if (!currentUser)
                    cout << "Invalid credentials.\n";
            }
        }
        else {
            if (currentUser->getRole() == "admin") {
                cout << "\n--- ADMIN MENU ---\n";
                cout << "1. View Users\n2. Delete User\n3. Logout\n";
                cin >> choice;

                if (choice == 1) {
                    for (const auto& u : auth.getAllUsers())
                        cout << u.getUsername() << " (" << u.getRole() << ")\n";
                }
                else if (choice == 2) {
                    string del;
                    cout << "Username to delete: ";
                    cin >> del;
                    auth.deleteUser(del);
                    cout << "User deleted.\n";
                }
                else if (choice == 3) {
                    currentUser = nullptr;
                }
            }
            else {
                cout << "\n--- USER MENU ---\n";
                cout << "1. View Profile\n2. Logout\n";
                cin >> choice;

                if (choice == 1) {
                    cout << "Username: " << currentUser->getUsername() << endl;
                    cout << "Role: " << currentUser->getRole() << endl;
                }
                else if (choice == 2) {
                    currentUser = nullptr;
                }
            }
        }

    } while (choice != 3);

    return 0;
}
