#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream> // Added for saving files

using namespace std;

class Member {
private:
    int id;
    string name;
    bool isPremium;
    bool hasPaid;

public:
    Member(int i, string n, bool premium)
        : id(i), name(n), isPremium(premium), hasPaid(true) {}

    int getId() const { return id; }
    string getName() const { return name; }
    bool getIsPremium() const { return isPremium; }
    bool getHasPaid() const { return hasPaid; }

    void setPaymentStatus(bool status) {
        hasPaid = status;
    }
};

class GymManager {
private:
    vector<Member> members;
    vector<int> currentlyInGym;
    vector<string> history;
    int maxCapacity = 10;

public:
    void registerMember(Member m) {
        members.push_back(m);
        cout << "Registered: " << m.getName() << endl;
    }

    void checkIn(int memberId) {
        if (currentlyInGym.size() >= maxCapacity) {
            cout << "DENIED: Gym at max capacity (" << maxCapacity << ")." << endl;
            return;
        }

        for (const auto& m : members) {
            if (m.getId() == memberId) {
                if (!m.getHasPaid()) {
                    cout << "DENIED: " << m.getName() << " has unpaid fees." << endl;
                    return;
                }

                currentlyInGym.push_back(memberId);

                // FIXED: Added history logging for check-in
                history.push_back("Member " + to_string(memberId) + " (" + m.getName() + ") checked in.");

                cout << "WELCOME: " << m.getName() << " is now checked in." << endl;
                return;
            }
        }
        cout << "ERROR: Member ID " << memberId << " not found." << endl;
    }

    void checkOut(int memberId) {
        for (auto it = currentlyInGym.begin(); it != currentlyInGym.end(); ++it) {
            if (*it == memberId) {
                currentlyInGym.erase(it);

                string name = "Unknown";
                for(const auto& m : members) if(m.getId() == memberId) name = m.getName();

                history.push_back("Member " + to_string(memberId) + " (" + name + ") checked out.");

                cout << "GOODBYE: " << name << " has left the gym." << endl;
                return;
            }
        }
        cout << "ERROR: Member " << memberId << " was not in the gym." << endl;
    }

    void generateReport() {
        cout << "\n================================" << endl;
        cout << "   DAILY GYM ACTIVITY REPORT" << endl;
        cout << "================================" << endl;

        if (history.empty()) {
            cout << "No activity recorded today." << endl;
        } else {
            for (int i = 0; i < (int)history.size(); i++) {
                cout << i + 1 << ". " << history[i] << endl;
            }
        }
        cout << "================================" << endl;
    }

    // NEW FEATURE: Saves the history to a physical file
    void saveReportToFile() {
        ofstream outFile("gym_activity_report.txt");
        if (outFile.is_open()) {
            outFile << "--- GYM ACTIVITY LOG ---" << endl;
            for (const string& log : history) {
                outFile << log << endl;
            }
            outFile.close();
            cout << "[System] Report successfully saved to gym_activity_report.txt" << endl;
        } else {
            cout << "[Error] Could not save the report file." << endl;
        }
    }

    void showStatus() {
        cout << "\n--- GYM LIVE STATUS ---" << endl;
        cout << "Current Occupancy: " << currentlyInGym.size() << "/" << maxCapacity << endl;
    }
}; // FIXED: Only one closing brace here to keep all functions inside the class

int main() {
    GymManager gym;

    // Registering members
    Member m1(1, "Ayush", true);
    Member m2(2, "Bob", true);
    gym.registerMember(m1);
    gym.registerMember(m2);

    // Simulating activity
    gym.checkIn(1);     // Ayush enters
    gym.checkOut(1);    // Ayush leaves
    gym.checkIn(2);     // Bob enters

    // Show current state
    gym.showStatus();

    // Generate and Save Reports
    gym.generateReport();
    gym.saveReportToFile();

    return 0;
}