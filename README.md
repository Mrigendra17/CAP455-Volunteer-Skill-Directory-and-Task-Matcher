# Volunteer Skill Directory

A C++ console application that stores volunteer records and finds volunteers by ID or by skill.

**Course:** CAP455: Object-Oriented Programming using C++ (MCA), Session 2026-27
**Project phase:** CA1, Design and Create (Units I-II)
**Student:** [Your name]  |  **Roll number:** [Your roll number]  |  **Faculty:** [Faculty name]

---

## Problem

NGOs and community groups often keep volunteer details in notebooks or scattered sheets, so finding someone with a needed skill is slow and records get duplicated or left incomplete. This program keeps volunteer records in one place, checks every entry, and lets an organiser search by ID or skill.

## Entity and unique key

| Item | Detail |
| --- | --- |
| Main class | `Volunteer` |
| Unique key | `volunteerId` (positive integer, no duplicates) |
| Storage in CA1 | Array of 100 `Volunteer` objects, in memory only |

### Volunteer data

| Field | Type | Rule |
| --- | --- | --- |
| `volunteerId` | `int` | Greater than 0 and unique |
| `name` | `std::string` | Not empty |
| `age` | `int` | 18 to 80 |
| `contact` | `std::string` | Exactly 10 digits |
| `skills` | `std::string` | Not empty, comma separated |
| `totalVolunteers` | `static int` | Count of volunteers added |

## Features by phase

| Phase | Status | What it covers |
| --- | --- | --- |
| **CA1** (Units I-II) | Done | Entity class with static, inline and friend members; array of objects; pointers; `this`; `std::string`; add, display all, search by ID, search by skill; input validation; duplicate-ID check |
| CA2 (Unit III) | Planned | Default, parameterised and copy constructors; destructor; binary file storage with `fstream`; search with `seekg`; basic update with `seekp`; Task entity |
| CA3 (Units IV-V) | Planned | Base class with at least two derived classes; operator overloading; type conversion; `new`/`delete`; virtual functions and destructor; safe update and delete; task matching |
| CA4 (Unit VI) | Planned | Full CRUD menu; custom exception class with `try`/`catch`/`throw`; templates; STL `vector` or `list` with `sort`, `find_if`, `count_if`; at least one advanced feature |

Menu options 5, 6 and 7 (Add Task, Search Task, Match Task) are placeholders in CA1 and print a message that the feature is reserved for a later phase.

## CA1 concepts used in the code

- **Encapsulation:** all `Volunteer` data members are private.
- **Static member:** `totalVolunteers`, updated by static functions `incrementCount()` and `showCount()`.
- **Inline functions:** `getId()` and `getSkills()`.
- **Friend function:** `hasSkill()` reads the private `skills` string for skill search.
- **`this` pointer:** used in `input()` when assigning members.
- **Array of objects and pointer:** `Volunteer volunteers[MAX_VOLUNTEERS]` is walked with `Volunteer* volunteerPtr`.
- **`std::string`:** used for text fields and for `find()` / `find_first_not_of()` validation.
- **`const` member functions:** `valid()`, `display()`, `getId()`, `getSkills()`.

## Build and run

Needs a standard C++ compiler (g++ with C++11 or later).

```bash
g++ -std=c++11 -o volunteer_directory src/main.cpp
./volunteer_directory
```

On Windows with MinGW, run `volunteer_directory.exe` instead.

## Menu

```
1. Add Volunteer
2. Display All Volunteers
3. Search Volunteer by ID
4. Search Volunteer by Skill
5. Add Task                      (reserved for CA2)
6. Search Task                   (reserved for CA2)
7. Match Task with Volunteers    (reserved for CA3)
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

Volunteer Added Successfully!

Enter Choice: 4

Enter Required Skill: Driving

Volunteer ID: 101
Name: Asha Verma
Age: 25
Contact: 9876543210
Skills: First Aid, Driving
```

## Validation

A record is saved only when all of these pass:

1. Age is a number from 18 to 80 (asked again until valid).
2. Contact is exactly 10 digits (asked again until valid).
3. `valid()` passes: ID above 0, name and skills not empty.
4. The ID is not already in the directory.

The directory holds at most 100 records; adding more prints "Volunteer Directory is Full!".

## Testing

The test table (normal, invalid and boundary cases) is in the CA1 report under `/docs`. Cases include ages 17, 18, 80 and 81, non-numeric age, short and non-digit contact numbers, ID 0, duplicate IDs, searching for a missing ID or skill, and an invalid menu choice.

## Known limitations (CA1)

- Records are lost when the program exits. File storage arrives in CA2.
- Typing letters at the menu or at an ID prompt is not handled; CA4 adds exception handling.
- Skill search is case-sensitive and matches substrings.
- The ID is checked after all fields are entered.
- No update or delete yet (basic update in CA2, safe update and delete in CA3, full CRUD in CA4).

## Repository layout

```
/docs    proposal, data dictionary, UML, flowcharts, CA1 report
/src     main.cpp
/data    sample data files (added in CA2)
/tests   test table and screenshots
README.md
```

## Academic integrity

This is individual work. [State exactly which books, online references or AI tools you used and for what, or write "None". The course requires this disclosure, and you must be able to explain and modify every line of code.]
