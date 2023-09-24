// Course : C++ Programming
// Desc : The Group allocation problem
// Uni : University of Leicester
// Assignment : Assignment 2B
// Version : 1.0

#include "allocation.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::ifstream;
using std::invalid_argument;
using std::istream;
using std::istringstream;
using std::map;
using std::ostream;
using std::pair;
using std::set;
using std::stringstream;

int main(int argc, char **argv)
{

    // storing names of the files
    vector<string> file_names = {"supervisors", "projects", "students", "allocations"};

    // using vector of pairs to store all files with their name
    // for example : ["supervisors" : "supervisors.txt"]
    vector<pair<string, string>> files;

    // let's catch the arguments from the command line
    try
    {
        // string stream for printing arguments
        stringstream ss;
        //  at least be 5 args (initial arg is program name itself (https://bit.ly/3FY2kla), and 4 arguments are files)
        bool is_valid_num_of_files = helper::check_num_of_files_input(argc, argv, ss);

        // check if the number of files is correct then store them in the vector of pairs
        if (is_valid_num_of_files)
        {
            // loop through the arguments and store them in the vector of pairs
            // indenxx 0
            for (int i = 1; i < argc; i++)
            {
                // store them in the map (starting with index 1)
                // files[file_names.at(i + 1) + "_file"] = argv[i];
                files.emplace_back(file_names[i - 1], argv[i]);
            }
        }
    }
    // catch any exception and print the error message
    catch (const exception &e)
    {
        // print the error message
        cerr << "Error " << e.what() << endl;
        return 1;
    }

    // ==========

    // ===================== project Allocation instance -========-
    // Project Allocation system
    auto &project_allocation_system = ProjectAlloc::get_instance();

    // -========-

    // ================ Opening files and reading them -========-
    try
    {
        // opening files and reading them)
        bool is_obj_created = helper::read_file_and_create_obj(files);

        // before start allocation let's check if the objects are created or not
        // throw an exception if the objects are not created
        if (!is_obj_created)
        {
            cerr << "Objects are not created!. Something went wrong" << endl;
            throw std::runtime_error("Objects are not created!");
        }
    } // catch any exception and print the error message
    catch (const exception &e)
    {
        // print the error message
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    // -================   let's start the project allocation  -========- //

    try
    {
        // let's start the allocation
        project_allocation_system.start_project_allocation_system();
    }
    catch (const exception &e)
    {
        // print the error message
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}