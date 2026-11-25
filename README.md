# StudentHelp – Peer Skill & Study Matching System

**Course:** Programming Fundamentals (Section BCS-1B)  
**University:** National University of Computer & Emerging Sciences, Karachi Campus  
**Department:** Department of Computer Science  
**Semester:** Fall 2026  
**Submitted to:** Ma'am Hijaab Sikandar

## Group Members

| Member | Roll No. | Contribution |
|---|---|---|
| Muhammad Ali | 26K-0548 | Student registration, profile creation, update and removal, profile display, saving records |
| Ahmad Raza | 26K-0609 | Subject searching, full and partial matching, mutual study partners |
| Taha Hamza | 26K-0566 | Main menu, loading records, menu choice handling, subject report |

## About the Project

StudentHelp is a console-based program written in C that helps students find peers who can teach particular subjects. Each student profile stores a numeric ID, a name, up to five subjects the student can teach, and up to five subjects the student needs help with.

Students often find academic help through friends or informal messages, which leaves no searchable record of who can teach what. StudentHelp provides a simple computerized record that can be searched and matched.

The supplied code combines all three members' contributions in one program with one `main()` function and a shared `Student` structure.

## Features

- Register a student with a duplicate-ID check.
- Update a student's name, teaching subjects, or needed subjects.
- Remove a profile using its student ID.
- Display all registered students and their subjects.
- Search for students who can teach a subject, ignoring letter case.
- Find full matches who can teach every subject a student needs.
- Find partial matches who can teach some of the needed subjects.
- Display mutual study partners who can each help the other with at least one subject.
- Show a subject report with the total number of students, teachers, and students needing help.
- Load records from `Students.txt` at startup and save them when exiting through option `0`.

## Concepts Used

Structures, arrays, strings, pointers, loops, decision structures, functions, and text-file handling. The program uses the standard headers `stdio.h`, `string.h`, and `ctype.h`.

## Project Structure

Save the combined C source as `main.c` in the project folder:

```text
StudentHelp/
    README.md
    main.c
    Students.txt
```

`Students.txt` is optional on the first run. The program creates or overwrites it when the user exits normally. The source filename `main.c` is used for the compilation instructions below.

## Requirements

- A C compiler supporting C99 or later, such as GCC.
- A terminal or an IDE such as Code::Blocks or Dev-C++.

## How to Compile and Run

Save the supplied code as plain C source in `main.c`, without Markdown formatting.

Compile from the project folder:

```sh
gcc -std=c99 main.c -o studenthelp
```

On Windows PowerShell, compile and run with:

```powershell
gcc -std=c99 main.c -o studenthelp.exe
.\studenthelp.exe
```

On Linux or macOS, run with:

```sh
./studenthelp
```

Run the program from the folder where you want `Students.txt` to be read and saved.

## Main Menu

| Option | Action |
|---|---|
| 1 | Register student |
| 2 | Update profile |
| 3 | Remove profile |
| 4 | Display all students |
| 5 | Search teacher by subject |
| 6 | Show matches for a student |
| 7 | Show mutual study partners |
| 8 | Subject report |
| 0 | Exit and save records |

The update submenu offers `1` to change the name, `2` to change teaching subjects, `3` to change needed subjects, and `4` to finish updating.

## Program Flow

1. Try to load existing records from `Students.txt`.
2. Display the main menu.
3. Read the user's choice and perform the selected action.
4. Return to the menu after each action.
5. When the user enters `0`, print `Byee!`, save the records, and exit.

If `Students.txt` cannot be opened at startup, the program starts with no students and displays the menu. It does not load sample students or print a missing-file message.

## Student Data and Limits

| Field | Storage |
|---|---|
| Student ID | Integer, such as `1` or `101` |
| Name | Array of 50 characters, including the terminating null character |
| Teaching subjects | Up to 5 subjects |
| Needed subjects | Up to 5 subjects |
| Each subject name | Array of 30 characters, including the terminating null character |
| Student capacity | 100 profiles |

Use numeric IDs in the program. Roll numbers such as `26K-0566` identify group members in this README but are not valid inputs for the program's integer ID field.

## Searching and Matching

Subject comparisons ignore letter case. For example, `calculus` matches `Calculus`. The complete subject name must match; the program does not perform partial-word searching or remove extra spaces.

- **Full match:** Another student can teach all subjects the selected student needs.
- **Partial match:** Another student can teach at least one, but fewer than all, of the needed subjects.
- **Mutual study partners:** Each student can teach at least one subject needed by the other. Each pair is displayed once.

The matching function excludes the selected student's own profile. If that student has no needed subjects, it displays a message instead of listing matches.

## Text-File Format

The filename used by the code is `Students.txt`, with a capital `S`. Each student occupies several lines in this order:

1. Numeric student ID.
2. Student name.
3. Number of teaching subjects.
4. Each teaching subject on its own line.
5. Number of needed subjects.
6. Each needed subject on its own line.

For the student registered in Test Case 2, the saved record is:

```text
1
Taha
2
Calculus
C Programming
1
English
```

Additional student records follow immediately in the same format. There are no pipe separators, comma-separated subject lists, or total-student-count header. When a subject count is zero, no subject lines follow that count.

## Test Cases

These three cases replace the earlier module-specific tests and correspond to the supplied Word document screenshots. The expected results below agree with the visible screenshot outputs; they are not a claim of a new execution of the program.

### Test Case 1: Main Menu Display

**Setup:** Start the program. For the screenshot's initial state, use a folder without an existing `Students.txt` file.

**Input:** Run the program without entering a menu choice yet.

**Expected result:** The main menu displays all options and waits for input:

```text
===== StudentHelp =====
1. Register student
2. Update profile
3. Remove profile
4. Display all students
5. Search teacher by subject
6. Show matches for a student
7. Show mutual study partners
8. Subject report
0. Exit
Enter choice:
```

**Screenshot observation:** All menu options and the input prompt are visible.

### Test Case 2: Successful Student Registration

**Setup:** The program is running, space is available, and student ID `1` is not already registered.

**Input:**

| Prompt | Value entered |
|---|---|
| Enter choice | `1` |
| Enter student ID | `1` |
| Enter student name | `Taha` |
| How many subjects can the student teach? | `2` |
| Teaching subject 1 | `Calculus` |
| Teaching subject 2 | `C Programming` |
| How many subjects does the student need help in? | `1` |
| Needed subject 1 | `English` |

**Expected result:** The program adds the profile and prints:

```text
Student registered successfully!
```

**Screenshot observation:** The entered ID, name, teaching subjects, needed subject, and success message are visible.

The profile remains in memory during the session. Choose `0` when finished to save it to `Students.txt`.

### Test Case 3: Invalid Menu Choice

**Setup:** The main menu is displayed.

**Input:** Enter `10` at the `Enter choice:` prompt.

**Expected result:** The program prints:

```text
Enter choice: 10
Invalid choice!
```

It then displays the main menu again so the user can enter another choice.

**Screenshot observation:** The input `10` and the message `Invalid choice!` are visible.

## Current Limitations

- The program rejects duplicate IDs during registration and numeric menu choices outside the available options, but it does not fully validate nonnumeric input.
- Subject counts must be entered between `0` and `5`. The current code prompts for this range but does not enforce it.
- Loaded file records are not fully validated for malformed data, duplicate IDs, or invalid subject counts. Use the documented format when editing `Students.txt` manually.
- Changes are saved only when exiting normally through option `0`; closing the program early can lose changes from the current session.
- Names and subject names must fit within their fixed-size arrays.
