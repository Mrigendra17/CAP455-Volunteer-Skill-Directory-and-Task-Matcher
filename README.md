# Volunteer Skill Directory

A C++ console application that stores volunteer and task records in binary files and finds volunteers by ID or by skill.

**Course:** CAP455: Object-Oriented Programming using C++ (MCA), Session 2026-27
**Project phase:** CA2, Constructors, File Handling and Task Entity (Unit III), building on CA1 (Units I-II)
**Student:** Mrigendra Singh |  **Roll number:** 68  |  **Faculty:** Adarsh Singh

---

## Problem

NGOs and community groups often keep volunteer details in notebooks or scattered sheets, so finding someone with a needed skill is slow and records get duplicated or left incomplete. This program keeps volunteer records in one place, checks every entry, and lets an organiser search by ID or skill.

## Entities and unique keys

| Item | Volunteer | Task |
| --- | --- | --- |
| Class | `Volunteer` | `Task` (new in CA2) |
| Unique key | `volunteerId` (1 to 999999, no duplicates) | `taskId` (1 to 999999, no duplicates) |
| Storage in CA1 | Array of 100 objects, in memory only | None |
| Storage in CA2 | `data/volunteers.dat` (binary file) | `data/tasks.dat` (binary file) |

### Volunteer data

| Field | Type | Rule |
| --- | --- | --- |
| `volunteerId` | `int` | 1 to 999999 and unique |
| `name` | `std::string` | Not empty, up to 49 characters |
| `age` | `int` | 18 to 80 |
| `contact` | `std::string` | Exactly 10 digits |
| `skills` | `std::string` | Not empty, comma separated, up to 99 characters |
| `totalVolunteers` | `static int` | Count of volunteers saved in the file |
| `activeObjects` | `static int` | Count of `Volunteer` objects currently in memory |

### Task data

| Field | Type | Rule |
| --- | --- | --- |
| `taskId` | `int` | 1 to 999999 and unique |
| `title` | `std::string` | Not empty, up to 59 characters |
| `requiredSkill` | `std::string` | Not empty, up to 99 characters |
| `volunteersNeeded` | `int` | 1 to 50 |

## Features by phase

| Phase | Status | What it covers |
| --- | --- | --- |
| **CA1** (Units I-II) | Done | Entity class with static, inline and friend members; array of objects; pointers; `this`; `std::string`; add, display all, search by ID, search by skill; input validation; duplicate-ID check |
| **CA2** (Unit III) | Done | Default, parameterised and copy constructors; destructor; binary file storage with `fstream`; search with `seekg`; basic update with `seekp`; Task entity; text export on exit |
| CA3 (Units IV-V) | Planned | Base class with at least two derived classes; operator overloading; type conversion; `new`/`delete`; virtual functions and destructor; safe update and delete; task matching |
| CA4 (Unit VI) | Planned | Full CRUD menu; custom exception class with `try`/`catch`/`throw`; templates; STL `vector` or `list` with `sort`, `find_if`, `count_if`; at least one advanced feature |

## Changes in CA2 (Unit III)

CA2 builds on the CA1 program. The `Volunteer` class and all CA1 features are still there; the changes below are what CA2 adds or replaces.

### Constructors and destructor

- **Default constructor:** creates an empty `Volunteer` (ID 0, age 0, empty strings).
- **Parameterised constructor:** creates a `Volunteer` from ID, name, age, contact and skills. It is used when a record is read back from a file.
- **Copy constructor:** creates a new `Volunteer` from an existing one. `updateVolunteer()` uses it to keep the old details so they can be shown before the change.
- **Copy assignment (`operator=`):** copies all fields when one `Volunteer` is assigned to another.
- **Destructor:** `~Volunteer()` reduces the new static counter `activeObjects`. The count is printed when the program exits ("Volunteer objects still in memory"), which shows that every object was destroyed.
- `Task` also has default and parameterised constructors.

### Binary file storage with `fstream`

- Volunteers are saved in `data/volunteers.dat` and tasks in `data/tasks.dat`, so records are **no longer lost when the program exits**.
- Each record is a fixed-size struct (`VolunteerRecord`, `TaskRecord`). Record number `i` always starts at byte `i * sizeof(record)`.
- New records are added with `ios::app`. `Volunteer::toRecord()` and `Volunteer::fromRecord()` (and the same pair in `Task`) convert between objects and file records.
- At startup the program creates the `data` folder and files if they are missing, then sets `totalVolunteers` from the number of records already in the file.
- The CA1 limit of 100 volunteers is gone, because records are no longer held in an array.
- **Text export:** on exit, all volunteers and tasks are also written to a readable file, `data/directory_export.txt`.

### Search and update with file pointers

- **`seekg`:** `readRecord()` jumps straight to record number `i` and reads it. It is used by search by ID, search by skill, display all and task search.
- **`seekp`:** `overwriteVolunteer()` moves to the record's position and overwrites it in place.
- **Update Volunteer (new menu option 7):** finds the volunteer by ID, shows the current details, then lets the user change the contact and skills. Pressing Enter keeps the old value. The new details are validated before they are written.

### Task entity

- New `Task` class with `taskId`, `title`, `requiredSkill` and `volunteersNeeded`.
- **Add Task** (5), **Search Task by ID** (6) and **Display All Tasks** (8) now work. Duplicate task IDs are rejected.
- **Match Task with Volunteers** is still reserved for CA3.

### Input handling

- Every input function asks again until the value is valid, so letters typed at the menu or an ID prompt now print "Invalid input! Enter numbers only." instead of breaking the program.
- Text fields cannot be empty and have a maximum length that fits the file record.
- If input ends (for example Ctrl+D), the program exits cleanly and still writes the export file.

### Other CA1 changes

- **Skill search ignores upper and lower case:** `hasSkill()` still reads the private `skills` string as a friend function, but now compares lowercase text, so "driving" finds "Driving".
- **Duplicate-ID check happens first:** the ID is checked against the file right after it is entered, before the other fields are asked.
- **`valid()`** now also checks age (18 to 80) and that the contact is 10 digits.
- **`display()` became `print(ostream&)`**, so the same function writes to the screen and to the export file.

## CA1 concepts still used in the code

- **Encapsulation:** all `Volunteer` and `Task` data members are private.
- **Static members:** `totalVolunteers` (with `setTotal()`, `incrementCount()`, `showCount()`) and `activeObjects` (with `showActiveObjects()`).
- **Inline functions:** `getId()` and `getSkills()`.
- **Friend function:** `hasSkill()`.
- **`this` pointer:** used in `input()`, `updateDetails()` and `operator=`.
- **Pointers:** `showVolunteers()` uses a `Volunteer*` to call `print()` and `hasSkill()`. The CA1 array of objects was replaced by file storage.
- **`std::string`:** used for all text fields, with `find()` and `find_first_not_of()` in validation.
- **`const` member functions:** `valid()`, `print()`, `getId()`, `getSkills()`, `toRecord()`.

## Build and run

Needs a standard C++ compiler (g++ with C++11 or later). The code is still a single file.

```bash
g++ -std=c++11 -o volunteer_directory src/main.cpp
./volunteer_directory
```

On Windows with MinGW, run `volunteer_directory.exe` instead. Run the program from the project's main folder, because the data files use the relative path `data/`.

## Menu

```
1. Add Volunteer
2. Display All Volunteers
3. Search Volunteer by ID
4. Search Volunteer by Skill
5. Add Task
6. Search Task by ID
7. Update Volunteer
8. Display All Tasks
9. Match Task with Volunteers    (reserved for CA3)
0. Exit
```

## Sample session

```
Enter Choice: 1

Enter Volunteer ID: 101
Enter Full Name: Asha Verma
Enter Age (18-80): 17
Age must be between 18 and 80.
Enter Age (18-80): 25
Enter Contact (10 digits): 98765
Invalid contact! Enter exactly 10 digits.
Enter Contact (10 digits): 9876543210
Enter Skills (comma separated): First Aid, Driving

Volunteer Added and Saved to File Successfully!

Enter Choice: 4

Enter Required Skill: driving

Volunteer ID: 101
Name: Asha Verma
Age: 25
Contact: 9876543210
Skills: First Aid, Driving

Enter Choice: 7

Enter Volunteer ID: 101

Current details:
Volunteer ID: 101
Name: Asha Verma
Age: 25
Contact: 9876543210
Skills: First Aid, Driving
Enter New Contact (10 digits, Enter to keep): 9123456780
Enter New Skills (Enter to keep):

Volunteer Updated Successfully!

Enter Choice: 5

Enter Task ID: 1
Enter Task Title: Medical Camp
Enter Required Skill: First Aid
Enter Volunteers Needed (1-50): 3

Task Added and Saved to File Successfully!

Enter Choice: 0

All records exported to data/directory_export.txt
Volunteer objects still in memory: 0

Thank You for Using Volunteer Skill Directory!
```

## Validation

A volunteer is saved only when all of these pass:

1. The ID is a number from 1 to 999999 and is not already in the file (checked first).
2. Name and skills are not empty and fit the length limit.
3. Age is a number from 18 to 80 (asked again until valid).
4. Contact is exactly 10 digits (asked again until valid).
5. `valid()` passes as a final check.

A task is saved only when its ID is new, title and skill are not empty, and volunteers needed is from 1 to 50.

## Testing

The test table (normal, invalid and boundary cases) is in the CA1 report under `/docs`. Cases include ages 17, 18, 80 and 81, non-numeric age, short and non-digit contact numbers, ID 0, duplicate IDs, searching for a missing ID or skill, and an invalid menu choice.

## Known limitations

- Update changes only the contact and skills. Name, age and ID cannot be changed, and there is no delete. Safe update and delete come in CA3, and the full CRUD menu in CA4.
- Skill search matches substrings (for example "aid" matches "First Aid").
- Tasks can be searched by ID only, and are not matched with volunteers until CA3.
- Input errors are handled with checks and loops. Custom exception classes arrive in CA4.
- The `.dat` files are raw binary, so they are meant to be read only by this program. Use `data/directory_export.txt` to read the records.

## Repository layout

```
/docs    proposal, data dictionary, UML, flowcharts, CA1 report
/src     main.cpp
/data    volunteers.dat, tasks.dat, directory_export.txt (created when the program runs)
/tests   test table and screenshots
README.md
```