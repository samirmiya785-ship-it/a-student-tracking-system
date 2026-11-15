# Student Tracking System

A command-line student tracker written in C. It stores a roster and records each student's attendance and homework status for 24 weeks.

## Requirements and running the program

You need a C compiler such as GCC. From the project folder, compile and run:

```powershell
gcc student.c -o student.exe
.\student.exe
```

The program asks for the password `teacher`. `database.txt` is read from and written to the program's current working directory. Run the program from the project folder so it uses the intended data file.

## Data model

The `Student` structure holds:

- `id`: the student's numeric ID.
- `name`: the student's name, up to 99 characters plus the terminating null character.
- `attend`: 24 attendance characters, one per week.
- `hw`: 24 homework characters, one per week.
- `progress`: calculated completion percentage.
- `status`: calculated performance category.

The `MAX_STUDENTS` and `TOTAL_WEEKS` constants set the capacity to 100 students and the tracking period to 24 weeks. The attendance and homework arrays have one extra character for the C string terminator.

Each week's attendance is `P` (present) or `A` (absent); homework is `S` (submitted) or `N` (not submitted). A new student's unrecorded weeks start as `-`.

## Menu

After a successful password check, the program loads student records and displays a menu:

1. **View Dashboard** - Shows the number of students, average progress, a count of students in each status category, and a weekly completion summary.
2. **Student Table** - Shows each student's ID, name, progress, status, and a compact summary for the first three weeks. `C` means attendance and homework are both complete; `P` means one is complete; `-` means neither is complete.
3. **Add New Student** - Reads a name, assigns an ID based on the current roster count, and initializes all 24 weeks to `-`.
4. **Attendance & Homework** - Selects a student by ID and a week from 1 to 24, then records attendance and homework. The letters are case-insensitive; only `P`/`A` and `S`/`N` are accepted.
5. **Save** - Writes all records to `database.txt` and exits.

Changes made during a session are held in memory until option 5 is selected. The program does not save automatically when another option is selected.

## Progress and status calculations

Each week can earn up to two points: one for `P` attendance and one for `S` homework. Across 24 weeks, the maximum is 48 points:

```text
progress = points / 48 * 100
```

The status is derived from progress:

- **Excellent**: 90% or higher
- **On Track**: 70% to less than 90%
- **Needs Attention**: 50% to less than 70%
- **Critical**: less than 50%

The dashboard's class average is the arithmetic mean of student progress. Its weekly `Completed` count means both attendance and homework are complete. `Partial` means exactly one is complete, and `Pending` means neither is complete. The weekly rate is the completed count divided by the number of students. With no students, the class average and weekly rates are shown as 0.

## Data file format

`database.txt` contains one pipe-delimited record per student:

```text
id|name|attendance-sequence|homework-sequence
```

For example, the attendance and homework fields each contain 24 characters, with each character representing one week. Progress and status are recalculated when records are loaded; they are not stored in the file.

The program reads at most 100 records. Keep the data file private if it contains real student information. This file is excluded by `.gitignore`.

## How the code is organized

- **Headers and constants** provide standard I/O, string operations, the student limit, and the number of weeks.
- **`Student`** defines the record stored for each student.
- **`clear_buffer`** consumes remaining input through the next newline so later prompts can read input cleanly.
- **`calculate_metrics`** counts attendance/homework points and updates progress and status.
- **`load_data`** opens `database.txt`, parses records, recalculates metrics, and fills the in-memory roster.
- **`save_data`** writes the roster in the pipe-delimited format and reports if the file cannot be opened.
- **`view_dashboard`** calculates class totals and prints the weekly summary.
- **`view_roster`** prints each student's calculated metrics and first three weekly completion codes.
- **`add_student`** checks the roster capacity, reads a name, initializes the weekly fields, and adds the student.
- **`update_student`** finds the requested ID, validates the week and status letters, updates that week's fields, and recalculates metrics.
- **`main`** authenticates the user, loads the roster, dispatches menu choices, and saves before exiting.

## Notes

- The password is hard-coded in the source and is suitable only for a basic classroom/demo program, not for protecting sensitive data.
- Student IDs are assigned as `count + 1`; the program does not provide deletion or ID-reuse handling.
- Each name is stored without a pipe (`|`) delimiter because pipes separate fields in the data file.
