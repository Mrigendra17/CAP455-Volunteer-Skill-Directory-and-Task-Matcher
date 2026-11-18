#include <iostream>
#include <fstream>
#include <string>
#include <cstring>    
#include <cstdlib>    
#include <cctype>     
using namespace std;

const string VOLUNTEER_FILE = "data/volunteers.dat";
const string TASK_FILE      = "data/tasks.dat";
const string EXPORT_FILE    = "data/directory_export.txt";

const int NAME_LEN = 50, CONTACT_LEN = 11, SKILLS_LEN = 100, TITLE_LEN = 60;

// Fixed-size records: record number i always starts at byte i * sizeof(record).
struct VolunteerRecord { int id; char name[NAME_LEN]; int age; char contact[CONTACT_LEN]; char skills[SKILLS_LEN]; };
struct TaskRecord      { int id; char title[TITLE_LEN]; char skill[SKILLS_LEN]; int needed; };

// ---------- Helpers ----------
string toLower(string s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = static_cast<char>(tolower(static_cast<unsigned char>(s[i])));
    return s;
}

bool allDigits(const string& s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); ++i)
        if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t");
    return a == string::npos ? "" : s.substr(a, s.find_last_not_of(" \t") - a + 1);
}

bool isTenDigits(const string& s) {
    return s.length() == 10 && allDigits(s);
}

// ---------- Input (every reader asks again until the value is valid) ----------
string readRaw(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) throw 0;        
    return trim(s);
}

int readInt(const string& prompt, int minV, int maxV, const string& rangeMsg) {
    while (true) {
        string s = readRaw(prompt);
        if (!allDigits(s) || s.size() > 9) {
            cout << "Invalid input! Enter numbers only.\n";
            continue;
        }
        int v = atoi(s.c_str());
        if (v >= minV && v <= maxV) return v;
        cout << rangeMsg << "\n";
    }
}

int readId(const string& what) {
    return readInt("\nEnter " + what + " ID: ", 1, 999999, what + " ID must be between 1 and 999999.");
}

string readLine(const string& prompt, size_t maxLen, bool allowBlank = false) {
    while (true) {
        string s = readRaw(prompt);
        if (s.empty() && allowBlank) return s;
        if (s.empty())          cout << "This field cannot be empty.\n";
        else if (s.size() > maxLen) cout << "Too long! Maximum " << maxLen << " characters.\n";
        else return s;
    }
}

string readContact(const string& prompt, bool allowBlank = false) {
    while (true) {
        string s = readRaw(prompt);
        if ((s.empty() && allowBlank) || isTenDigits(s)) return s;
        cout << "Invalid contact! Enter exactly 10 digits.\n";
    }
}

// ---------- Volunteer ----------
class Volunteer {
    int volunteerId;
    string name;
    int age;
    string contact;
    string skills;
    static int totalVolunteers;  
    static int activeObjects;     

public:
    Volunteer() : volunteerId(0), age(0) { ++activeObjects; }                       // default

    Volunteer(int id, const string& n, int a, const string& c, const string& s)     // parameterised
        : volunteerId(id), name(n), age(a), contact(c), skills(s) { ++activeObjects; }

    Volunteer(const Volunteer& o)                                                   // copy
        : volunteerId(o.volunteerId), name(o.name), age(o.age), contact(o.contact), skills(o.skills) {
        ++activeObjects;
    }

    Volunteer& operator=(const Volunteer& o) {
        if (this != &o) {
            this->volunteerId = o.volunteerId; this->name = o.name; this->age = o.age;
            this->contact = o.contact; this->skills = o.skills;
        }
        return *this;
    }

    ~Volunteer() { --activeObjects; }                                               // destructor

    void input(int id) {
        this->volunteerId = id;
        this->name    = readLine("Enter Full Name: ", NAME_LEN - 1);
        this->age     = readInt("Enter Age (18-80): ", 18, 80, "Age must be between 18 and 80.");
        this->contact = readContact("Enter Contact (10 digits): ");
        this->skills  = readLine("Enter Skills (comma separated): ", SKILLS_LEN - 1);
    }

    // Basic update: Enter keeps the old value.
    void updateDetails() {
        string c = readContact("Enter New Contact (10 digits, Enter to keep): ", true);
        string s = readLine("Enter New Skills (Enter to keep): ", SKILLS_LEN - 1, true);
        if (!c.empty()) this->contact = c;
        if (!s.empty()) this->skills = s;
    }

    inline int getId() const { return volunteerId; }
    inline string getSkills() const { return skills; }

    bool valid() const {
        return volunteerId > 0 && age >= 18 && age <= 80 && !name.empty() &&
               isTenDigits(contact) && !skills.empty();
    }

    void print(ostream& out) const {      // screen or text file
        out << "\nVolunteer ID: " << volunteerId << "\nName: " << name << "\nAge: " << age
            << "\nContact: " << contact << "\nSkills: " << skills << "\n";
    }

    VolunteerRecord toRecord() const {
        VolunteerRecord r;
        memset(&r, 0, sizeof(r));
        r.id = volunteerId;  r.age = age;
        strncpy(r.name, name.c_str(), NAME_LEN - 1);
        strncpy(r.contact, contact.c_str(), CONTACT_LEN - 1);
        strncpy(r.skills, skills.c_str(), SKILLS_LEN - 1);
        return r;
    }

    static Volunteer fromRecord(const VolunteerRecord& r) {
        return Volunteer(r.id, r.name, r.age, r.contact, r.skills);
    }

    static void setTotal(int n) { totalVolunteers = n; }
    static void incrementCount() { ++totalVolunteers; }
    static void showCount() { cout << "\nTotal Volunteers Saved: " << totalVolunteers << "\n"; }
    static void showActiveObjects() { cout << "Volunteer objects still in memory: " << activeObjects << "\n"; }

    friend bool hasSkill(const Volunteer&, const string&);
};

int Volunteer::totalVolunteers = 0;
int Volunteer::activeObjects = 0;

bool hasSkill(const Volunteer& v, const string& skill) {     // ignores upper/lower case
    return toLower(v.skills).find(toLower(skill)) != string::npos;
}

// ---------- Task ----------
class Task {
    int taskId;
    string title;
    string requiredSkill;
    int volunteersNeeded;

public:
    Task() : taskId(0), volunteersNeeded(0) {}
    Task(int id, const string& t, const string& s, int n)
        : taskId(id), title(t), requiredSkill(s), volunteersNeeded(n) {}

    void input(int id) {
        this->taskId = id;
        this->title = readLine("Enter Task Title: ", TITLE_LEN - 1);
        this->requiredSkill = readLine("Enter Required Skill: ", SKILLS_LEN - 1);
        this->volunteersNeeded = readInt("Enter Volunteers Needed (1-50): ", 1, 50,
                                         "Volunteers needed must be between 1 and 50.");
    }

    void print(ostream& out) const {
        out << "\nTask ID: " << taskId << "\nTitle: " << title << "\nRequired Skill: " << requiredSkill
            << "\nVolunteers Needed: " << volunteersNeeded << "\n";
    }

    TaskRecord toRecord() const {
        TaskRecord r;
        memset(&r, 0, sizeof(r));
        r.id = taskId;  r.needed = volunteersNeeded;
        strncpy(r.title, title.c_str(), TITLE_LEN - 1);
        strncpy(r.skill, requiredSkill.c_str(), SKILLS_LEN - 1);
        return r;
    }

    static Task fromRecord(const TaskRecord& r) { return Task(r.id, r.title, r.skill, r.needed); }
};

// ---------- File handling ----------
int recordCount(const string& path, size_t size) {
    ifstream f(path.c_str(), ios::binary | ios::ate);
    return f ? static_cast<int>(static_cast<size_t>(f.tellg()) / size) : 0;
}

// seekg: jump straight to record number i and read it. False at end of file.
bool readRecord(ifstream& f, int i, void* rec, size_t size) {
    f.seekg(static_cast<streamoff>(i) * size, ios::beg);
    return static_cast<bool>(f.read(static_cast<char*>(rec), size));
}

bool appendRecord(const string& path, const void* rec, size_t size) {
    ofstream f(path.c_str(), ios::binary | ios::app);
    return f.write(static_cast<const char*>(rec), size).good();
}

bool findVolunteerById(int id, Volunteer* out, int* index) {
    ifstream f(VOLUNTEER_FILE.c_str(), ios::binary);
    VolunteerRecord r;
    for (int i = 0; readRecord(f, i, &r, sizeof(r)); ++i) {
        if (r.id == id) {
            if (out) *out = Volunteer::fromRecord(r);
            if (index) *index = i;
            return true;
        }
    }
    return false;
}

// seekp: overwrite record number `index` in place.
bool overwriteVolunteer(int index, const Volunteer& v) {
    fstream f(VOLUNTEER_FILE.c_str(), ios::in | ios::out | ios::binary);
    VolunteerRecord r = v.toRecord();
    f.seekp(static_cast<streamoff>(index) * sizeof(r), ios::beg);
    return f.write(reinterpret_cast<const char*>(&r), sizeof(r)).flush().good();
}

bool findTaskById(int id, Task* out) {
    ifstream f(TASK_FILE.c_str(), ios::binary);
    TaskRecord r;
    for (int i = 0; readRecord(f, i, &r, sizeof(r)); ++i) {
        if (r.id == id) {
            if (out) *out = Task::fromRecord(r);
            return true;
        }
    }
    return false;
}

// Prints saved volunteers (only those with `skill`, if given). Returns how many.
int showVolunteers(const string& skill, ostream& out) {
    ifstream f(VOLUNTEER_FILE.c_str(), ios::binary);
    VolunteerRecord r;
    int shown = 0;
    for (int i = 0; readRecord(f, i, &r, sizeof(r)); ++i) {
        Volunteer v = Volunteer::fromRecord(r);
        Volunteer* p = &v;
        if (skill.empty() || hasSkill(*p, skill)) { p->print(out); ++shown; }
    }
    return shown;
}

int showTasks(ostream& out) {
    ifstream f(TASK_FILE.c_str(), ios::binary);
    TaskRecord r;
    int shown = 0;
    for (int i = 0; readRecord(f, i, &r, sizeof(r)); ++i) { Task::fromRecord(r).print(out); ++shown; }
    return shown;
}

// On exit: write everything from the .dat files into a readable text file.
void exportAllToText() {
    ofstream out(EXPORT_FILE.c_str());
    out << "VOLUNTEER SKILL DIRECTORY - DATA EXPORT\n";
    out << "\n===== VOLUNTEERS (" << recordCount(VOLUNTEER_FILE, sizeof(VolunteerRecord)) << ") =====\n";
    if (showVolunteers("", out) == 0) out << "\nNo Volunteers Available.\n";
    out << "\n===== TASKS (" << recordCount(TASK_FILE, sizeof(TaskRecord)) << ") =====\n";
    if (showTasks(out) == 0) out << "\nNo Tasks Available.\n";
    cout << "\nAll records exported to " << EXPORT_FILE << "\n";
}

// ---------- Menu actions ----------
void addVolunteer() {
    int id = readId("Volunteer");
    if (findVolunteerById(id, NULL, NULL)) { cout << "\nVolunteer ID Already Exists!\n"; return; }

    Volunteer temp;
    temp.input(id);
    if (!temp.valid()) { cout << "\nInvalid Volunteer Details!\n"; return; }

    VolunteerRecord r = temp.toRecord();
    if (appendRecord(VOLUNTEER_FILE, &r, sizeof(r))) {
        Volunteer::incrementCount();
        cout << "\nVolunteer Added and Saved to File Successfully!\n";
    } else {
        cout << "\nError: could not write to " << VOLUNTEER_FILE << "\n";
    }
}

void searchVolunteerById() {
    Volunteer found;
    int index = -1;
    if (findVolunteerById(readId("Volunteer"), &found, &index)) {
        cout << "\nFound at record number " << index + 1 << ":";
        found.print(cout);
    } else {
        cout << "\nVolunteer Not Found!\n";
    }
}

void updateVolunteer() {
    Volunteer current;
    int index = -1;
    if (!findVolunteerById(readId("Volunteer"), &current, &index)) { cout << "\nVolunteer Not Found!\n"; return; }

    Volunteer before(current);                 // copy constructor keeps the old values
    cout << "\nCurrent details:";
    before.print(cout);

    current.updateDetails();
    if (current.valid() && overwriteVolunteer(index, current)) {
        cout << "\nVolunteer Updated Successfully!\n\nUpdated details:";
        current.print(cout);
    } else {
        cout << "\nUpdate failed. Nothing was changed.\n";
    }
}

void addTask() {
    int id = readId("Task");
    if (findTaskById(id, NULL)) { cout << "\nTask ID Already Exists!\n"; return; }

    Task t;
    t.input(id);
    TaskRecord r = t.toRecord();
    cout << (appendRecord(TASK_FILE, &r, sizeof(r)) ? "\nTask Added and Saved to File Successfully!\n"
                                                      : "\nError: could not write to the task file.\n");
}

// ---------- Main ----------
int main() {
    // Opening in append mode creates a data file if it does not exist yet.
    ofstream v(VOLUNTEER_FILE.c_str(), ios::binary | ios::app);
    ofstream t(TASK_FILE.c_str(), ios::binary | ios::app);
    if (!v || !t) {
        // The data folder is missing: create it ("mkdir data" works on Windows and Linux/macOS), then retry.
        if (system("mkdir data") != 0) cout << "Could not create the data folder.\n";
        v.clear();  v.open(VOLUNTEER_FILE.c_str(), ios::binary | ios::app);
        t.clear();  t.open(TASK_FILE.c_str(), ios::binary | ios::app);
        if (!v || !t) { cout << "Cannot create the data files. Check folder permissions.\n"; return 1; }
    }
    v.close();
    t.close();

    Volunteer::setTotal(recordCount(VOLUNTEER_FILE, sizeof(VolunteerRecord)));   // from earlier runs

    int choice = -1;
    try {
        do {
            cout << "\n----- VOLUNTEER SKILL DIRECTORY------ "
                    "\n1. Add Volunteer\n2. Display All Volunteers\n3. Search Volunteer by ID"
                    "\n4. Search Volunteer by Skill\n5. Add Task\n6. Search Task by ID"
                    "\n7. Update Volunteer\n8. Display All Tasks\n9. Match Task with Volunteers\n0. Exit";
            choice = readInt("\n\nEnter Choice: ", 0, 9, "Invalid Menu Choice!");

            if (choice == 1) addVolunteer();
            else if (choice == 2) {
                if (showVolunteers("", cout) == 0) cout << "\nNo Volunteers Available.\n";
                Volunteer::showCount();
            }
            else if (choice == 3) searchVolunteerById();
            else if (choice == 4) {
                if (showVolunteers(readLine("\nEnter Required Skill: ", SKILLS_LEN - 1), cout) == 0)
                    cout << "\nNo Volunteer Found for This Skill.\n";
            }
            else if (choice == 5) addTask();
            else if (choice == 6) {
                Task found;
                if (findTaskById(readId("Task"), &found)) found.print(cout);
                else cout << "\nTask Not Found!\n";
            }
            else if (choice == 7) updateVolunteer();
            else if (choice == 8 && showTasks(cout) == 0) cout << "\nNo Tasks Available.\n";
            else if (choice == 9)
                cout << "\n    MATCH TASK WITH VOLUNTEERS:\nThis feature is reserved for CA3."
                        "\nAdvanced task matching using inheritance\nand polymorphism will be implemented"
                        "\nin a future phase.\n";

        } while (choice != 0);
    } catch (int) {
        cout << "\nInput ended. Exiting program.\n";
    }

    exportAllToText();
    Volunteer::showActiveObjects();
    cout << "\nThank You for Using Volunteer Skill Directory!\n";
    return 0;
}