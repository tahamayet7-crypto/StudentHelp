# StudentHelp – Peer Skill & Study Matching System

**Course:** Programming Fundamentals (Section BCS-1B)
**University:** National University of Computer & Emerging Sciences, Karachi Campus
**Department:** Department of Computer Science
**Semester:** Fall 2026
**Submitted to:** Ma'am Hijaab Sikandar

## Group Members

| Member | Roll No. | Module |
|---|---|---|
| Muhammad Ali | 26K-0548 | Student registration, profile creation, update and removal, display of all profiles |
| Ahmad Raza | 26K-0609 | Subject searching, peer-matching algorithm, mutual study partners |
| Taha Hamza | 26K-0566 | Main menu, text-file and loading, input validation, profile listing, subject report |

## About the Project

StudentHelp is a console-based program written in C that helps students find peers who can teach, or need help with, particular subjects. Each student profile stores an ID, a name, up to five subjects the student can teach and up to five subjects the student needs help with.

Students often find academic help through friends or informal messages, which leaves no searchable record of who can teach what. StudentHelp provides a simple computerized record that can be searched and matched.

## Features

- Student profile registration with duplicate-ID check
- Update and remove a profile
- Lists of subjects a student can teach and needs help with
- Search for students who can teach a given subject (case-insensitive)
- Peer matching: full and partial matches, and mutual study partners
- Text-file storage (`students.txt`) with validation of every record
- Simple menu-based navigation, profile listing and subject report

## Concepts Used

Structures, arrays, strings, loops, decision structures, functions and text-file handling.

## Project Structure

```
StudentHelp/
├── README.md
├── ali_registration/
│   └── profiles_main.c      (registration, update, removal, display)
├── raza_matching/
│   └── search.c             (searching, matching, mutual partners)
└── taha_menu_files/
    └── main.c               (menu, file handling, validation, reports)
```

## Requirements

- A C compiler such as GCC (also works with Code::Blocks or Dev C++)

## How to Compile and Run

Each module is currently a separate program. From the main project folder:

**Ali's module (registration and profile maintenance):**
```
gcc ali_registration/profiles_main.c -o ali
./ali
```

**Taha's module (menu, file handling, reports):**
```
gcc taha_menu_files/main.c -o taha
./taha
```

On Windows, run the programs as `ali.exe` and `taha.exe`.

**Ahmad's module (searching and matching):**
`search.c` has no `main()` and includes a shared header, `studenthelp.h`, which has not been written yet. It cannot be compiled on its own until the modules are integrated.

## Program Flow

Start → Load records from `students.txt` (if present) → Display Menu → Register / Update / Search / Match / List → Save to File → Exit

## Text-File Format

`students.txt` stores one student per line:

```
id|name|teachCount|teach1,teach2|needCount|need1,need2
```

Example:

```
26K-0001|Taha Mayet|2|Calculus,C Programming|1|Functional English
26K-0002|Ahmed Kazmi|1|Functional English|2|Calculus,C Programming
```

## Test Cases

### Test 1: No file, sample students load (Taha's module)

- **Setup:** No `students.txt` in the folder
- **Input:** Run `./taha`, choose `1`, then `0`
- **Expected:** Prints "Using sample students. Could not open students.txt." and "Loaded 2 student record.", then lists Taha Mayet and Ahmed Kazmi with their subjects.

### Test 2: Subject report is case-insensitive (Taha's module)

- **Setup:** Same as Test 1
- **Input:** Choose `2`, enter `CALCULUS`, then `0`
- **Expected:** Total students: 2, Can teach CALCULUS: 1, Needs help with CALCULUS: 1.

### Test 3: Bad record is rejected (Taha's module)

- **Setup:** `students.txt` contains one line where the count is wrong:
  `26K-0003|Sara Khan|3|Calculus,Physics|0|`
- **Input:** Run `./taha`
- **Expected:** Prints "Invalid record or too many students at line 1." and exits without showing the menu.

### Test 4: Duplicate ID is rejected (Ali's module)

- **Input:** Choose `1`, enter ID `101`, name `Ali`, `0` teach subjects, `0` needed subjects. Choose `1` again and enter ID `101`.
- **Expected:** First time prints "Student registered successfully!". Second time prints "ID already exists!" and no second profile is added.
