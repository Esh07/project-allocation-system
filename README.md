# Project Allocation System

This repository contains my implementation of a project allocation system using a greedy algorithm to solve the assignment in the CO3105/4105/7105 Advanced C++ Programming module at the University of Leicester.

- [Project Allocation System](#project-allocation-system)
  - [Problem](#problem)
    - [Constraints](#constraints)
    - [Scoring](#scoring)
    - [Solution](#solution)
    - [Program Input and Output](#program-input-and-output)
    - [Results](#results)
  - [Documentation](#documentation)
    - [Requirements](#requirements)
    - [Get project locally](#get-project-locally)
    - [Compilation](#compilation)
    - [Execution (With your own input files)](#execution-with-your-own-input-files)
    - [Test cases](#test-cases)
    - [Clean](#clean)

<details>
<summary>Detailed Problem Description</summary>

## Problem

Create a system to allocate students to supervisors and projects.

- Each supervisor has a ‘supervision load’ (number of students they can supervise) and proposes projects with different ‘multiplicities’ (maximum number of students per project).
- The system should ensure that the total project capacity per supervisor matches their load, and the total supervision capacity is enough for all students.

### Constraints

Based on this, an allocation of projects to students should be produced. The allocation produced must be legal. This means:

- Each student is allocated at most one project.
- The number of students allocated to a project does not exceed the multiplicity of that project.
- The total number of students allocated to a supervisor's proposed projects does not exceed the load of that supervisor.
- The allocation must also satisfy some local optimality requirements.

The allocation produced also has to satisfy some local optimality requirements. This means:

- Each student must be allocated a project, even if the allocated project is not on their preference list at all, unless no projects are available.
- No student should be allocated a project lower on their preference list (or not on their preference list at all) when there are other projects higher on their preference list that are not full.
- No two students would prefer swapping their allocated projects (i.e., both ranked the other student's allocated project higher in their own preference list).

### Scoring

- Each student getting his/her first choice gives a score of 4.
- Second choice a score of 3.
- Third choice a score of 2
- Fourth choice a score of 1
- And anything else gives a score of 0.
  > The score of the allocation is the sum of all student scores.

### Solution

My system uses a greedy algorithm to solve the project allocation system problem. The greedy algorithm works by iterating over the students in order of their preference lists and assigning each student to the highest-ranked project that is still available, subject to the constraints listed above.

### Program Input and Output

```bash
./GenAlloc staff.txt projects.txt students.txt alloc.txt
```

All three input files are text files, each consisting of a number of lines. Each line represents a supervisor/project/student, and contains a number of fields separated by whitespace, as follows:

- Supervisor file: Each line consists of a supervisor id, followed by his/her load.
- Project file: Each line consists of a project id, then supervisor id, then multiplicity, and finally the project title.
- Student file: Each line consists of a student id, followed by up to four integers which are the project ids of this student's choices in descending order of preference. Note that some students may make fewer than four choices or even no choices at all.

The result of the allocation must be written to the named output file. (If the file already exists, it is quietly overwritten.)

Each line contains a student id, followed by a space, followed by the id of the allocated project.

These lines should be in increasing alphanumeric order (the standard C++ string comparison order) of the student userid. There must be a line for each student; if a student is not allocated any project, then 0 is used in place of the project id. Finally, there is another line containing a single integer, which is the score of the allocation.

The four files must be in this order in the command line arguments (staff, projects, students, output). All these files are assumed to be in the same directory as the executable program. Do not hardcode any other paths.

</details>

### Results

My system was able to produce legal and locally optimal allocations for all of the sample input files provided. I also tested my system on a number of larger input files and achieved good results.

My implementation of the project allocation system is written in C++ and uses a variety of advanced C++ features, such as templates, classes, and object-oriented programming. I also used a makefile to automate the build and test process.

## Documentation

### Requirements

:warning: At least, `gcc 11` is required to compile this program.

I have used `C++17` (-std=c++17) STL features in this program (only those that are supported by gcc 11).

### Get project locally

```bash
git clone https://github.com/Esh07/project-allocation-system
```

### Compilation

To compile the program, run the following command in the root directory of the project:

```bash
make
```

> This will create an executable file named `GenAlloc` in the root directory of the project.

### Execution (With your own input files)

> NOTE: Please make sure that you've identical format of input files as mentioned in the problem statement.
> Input the 3 files - staff.txt, projects.txt, students.txt - in the same order as mentioned but you can name them as you want.

To run the program, run the following command in the root directory of the project:

At least 3 arguments required excluding the program name itself (./GenAlloc). In short, program at least need 4 (inc program name). You could add allloc.txt and it can handle it.

> :rotating_light: **NOTE**: below 4 or above 5 arguments will cause the program to exit with error. `<alloc_file>` is optional.

```bash
./GenAlloc <staff_file> <project_file> <student_file> <alloc_file>
```

> This will create an output file named `alloc.txt` in the root directory of the project. If the file already exists, it is quietly overwritten.

### Test cases

To run the test cases, run the following command in the root directory of the project:

> There are 13 test cases in total. You can run any of them by replacing `<testcase-number>` with the number of the test case you want to run.

```bash
make testcase-<testcase-number> # - e.g. make testcase-1
```

After running the test case, an output file alloc.txt is generated in the project’s root directory. This can be compared with the expected output in testcase-\<testcase-number>/4_alloc.txt.

## Clean

To clean the object files only, run the following command in the root directory of the project:

```bash
make clean # - clean project and remove object files
```

To clean the object files and the executable file, run the following command in the root directory of the project:

```bash
make deepclean  #- clean project and remove object files and executable files
```
