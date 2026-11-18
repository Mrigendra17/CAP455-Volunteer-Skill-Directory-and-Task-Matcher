#include <iostream>
#include <string>
using namespace std;

const int MAX_VOLUNTEERS = 100; // Limits for number of volunteers

class Volunteer {
private:
    int volunteerId;
    string name;
    int age;
    string contact;
    string skills;
    static int totalVolunteers;

public:
    Volunteer() : volunteerId(0), age(0) {}
    void input() {
        cout << "\nEnter Volunteer ID: ";
        cin >> this->volunteerId;

        cout << "Enter Full Name: ";
        cin >> ws;
        getline(cin, this->name);

        // Age validation
        // Age validation
    do {
        cout << "Enter Age (18-80): ";
    cin >> age;

    if (cin.fail()) {
        cout << "Invalid input! Enter numbers only.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        age = 0;
        }
    else if (age < 18 || age > 80) {
        cout << "Age must be between 18 and 80.\n";
        }

    } while (age < 18 || age > 80);

        // Contact validation
        cout << "Enter Contact (10 digits): ";
        cin >> this->contact;

    while (contact.length() != 10 ||
    contact.find_first_not_of("0123456789") != string::npos) {
        cout << "Invalid contact! Enter exactly 10 digits.\n";
        cout << "Enter Contact (10 digits): ";
        cin >> this->contact;
}

        cout << "Enter Skills (comma separated): ";
        cin >> ws;
        getline(cin, this->skills);
    }

    inline int getId() const {
        return volunteerId;
    }

    inline string getSkills() const {
        return skills;
    }

    bool valid() const {
        return volunteerId > 0 &&
               age >= 18 && age <= 80 &&
               !name.empty() &&
               contact.length() == 10 &&
               contact.find_first_not_of("0123456789") == string::npos &&
               !skills.empty();
    }

    void display() const {
        cout << "\nVolunteer ID: " << volunteerId
             << "\nName: " << name
             << "\nAge: " << age
             << "\nContact: " << contact
             << "\nSkills: " << skills << "\n";
    }

    static void incrementCount() {
        ++totalVolunteers;
    }

    static void showCount() {
        cout << "\nTotal Volunteers Added: "
             << totalVolunteers << "\n";
    }

    friend bool hasSkill(const Volunteer&, const string&);
};

int Volunteer::totalVolunteers = 0;


// Friend Function
bool hasSkill(const Volunteer& volunteer, const string& skill) {
    return volunteer.skills.find(skill) != string::npos;
}


// Main Function
int main() {

    Volunteer volunteers[MAX_VOLUNTEERS];
    Volunteer* volunteerPtr = volunteers;
    int volunteerCount = 0;
    int choice;

    do {
        cout << "\n----- VOLUNTEER SKILL DIRECTORY------ ";
        cout << "\n1. Add Volunteer";
        cout << "\n2. Display All Volunteers";
        cout << "\n3. Search Volunteer by ID";
        cout << "\n4. Search Volunteer by Skill";
        cout << "\n5. Add Task";
        cout << "\n6. Search Task";
        cout << "\n7. Match Task with Volunteers";
        cout << "\n0. Exit";

        cout << "\n\nEnter Choice: ";
        cin >> choice;


        // Add Volunteer
        if (choice == 1) {
            if (volunteerCount >= MAX_VOLUNTEERS) {
                cout << "\nVolunteer Directory is Full!\n";
                continue;
            }

            Volunteer temp;
            temp.input();

            bool duplicate = false;

            for (int i = 0; i < volunteerCount; ++i) {
                if (volunteerPtr[i].getId() == temp.getId()) {
                    duplicate = true;
                    break;
                }
            }

            if (!temp.valid()) {
                cout << "\nInvalid Volunteer Details!\n";
            }
            else if (duplicate) {
                cout << "\nVolunteer ID Already Exists!\n";
            }
            else {
                volunteers[volunteerCount] = temp;

                ++volunteerCount;

                Volunteer::incrementCount();

                cout << "\nVolunteer Added Successfully!\n";
            }
        }

        // Display All Volunteers
        else if (choice == 2) {

            if (volunteerCount == 0) {
                cout << "\nNo Volunteers Available.\n";
            }

            for (int i = 0; i < volunteerCount; ++i) {
                volunteerPtr[i].display();
            }

            Volunteer::showCount();
        }

        // Search Volunteer by ID
        else if (choice == 3) {

            int id;

            cout << "\nEnter Volunteer ID: ";
            cin >> id;

            bool found = false;

            for (int i = 0; i < volunteerCount; ++i) {

                if (volunteerPtr[i].getId() == id) {

                    volunteerPtr[i].display();

                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "\nVolunteer Not Found!\n";
            }
        }

        // Search Volunteer by Skill
        else if (choice == 4) {

            string skill;

            cout << "\nEnter Required Skill: ";
            cin >> ws;
            getline(cin, skill);

            bool found = false;

            for (int i = 0; i < volunteerCount; ++i) {

                if (hasSkill(volunteerPtr[i], skill)) {

                    volunteerPtr[i].display();

                    found = true;
                }
            }

            if (!found) {
                cout << "\nNo Volunteer Found for This Skill.\n";
            }
        }

        // Add Task - Future CA
        else if (choice == 5) {
            cout << "\n     ADD TASK:";
            cout << "\nThis feature is reserved for CA2.";
            cout << "\nTask creation and file storage will be";
            cout << "\nimplemented in the next phase.\n";
        }


        // Search Task - Future CA
        else if (choice == 6) {
            cout << "\n     SEARCH TASK:";
            cout << "\nThis feature is reserved for CA2.";
            cout << "\nTask searching and record retrieval will be";
            cout << "\nimplemented in the next phase.\n";
        }

        // Match Task - Future CA
        else if (choice == 7) {
            cout << "\n    MATCH TASK WITH VOLUNTEERS:";
            cout << "\nThis feature is reserved for CA3.";
            cout << "\nAdvanced task matching using inheritance";
            cout << "\nand polymorphism will be implemented";
            cout << "\nin a future phase.\n";
        }

        // Invalid Choice
        else if (choice != 0) {
            cout << "\nInvalid Menu Choice!\n";
        }

    } while (choice != 0);
    
    cout << "\nThank You for Using Volunteer Skill Directory!\n";
    return 0;
}
