#include "allocation.h"

#include <sstream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <string>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <any>
#include <random>
#include <fstream>

using std::any;
using std::any_cast;
using std::cerr;
using std::cout;
using std::distance;
using std::endl;
using std::exception;
using std::find;
using std::for_each;
using std::fstream;
using std::ifstream;
using std::invalid_argument;
using std::is_base_of;
using std::istringstream;
using std::make_unique;
using std::minus;
using std::move;
using std::ofstream;
using std::ostream;
using std::pair;
using std::remove_if;
using std::stringstream;
using std::to_string;
/// ========------- Helper functions (namespace )-------========///

namespace helper
{

    // helper function for checking number of files input
    bool check_num_of_files_input(int argc, char *argv[], stringstream &ss)
    {
        // if (argc != 5 )
        if (argc != 4 && argc != 5)
        {
            int files_count = 0;
            // ignore the first argument (program name) and loop through the rest of the arguments
            for (int i = 1; i < argc; i++)
            {
                ss << "argv[" << i << "] = " << argv[i] << " ";
                files_count++;
            }

            cerr << "Input file: ";
            (files_count == 0) ? cerr << "No files passed!" << endl : cerr << "Files passed: " << files_count << "expected total of at least 3 files!" << endl;
            // throw an exception if the number of arguments is not 4 (obviously 5 including the program name)
            throw invalid_argument("File is missing. Make sure you're at least passing 3 files!");
            return false;
        }

        // yes, there are 4 files
        return true;
    }

    // helper function for read file
    bool read_file(const string &file_name, ifstream &in_file)
    {

        // read the file line by line
        string line;

        while (getline(in_file, line))
        {

            // create an object from the line
            bool is_obj_created_and_move_to_list = ObjectFactory::create_object(file_name, line);

            // check if the object is not null
            if (is_obj_created_and_move_to_list != true)
            {
                cerr << "Error: Object is not created nor moved to ProjectAlloc list" << endl;
                return false;
            }
        }

        // yes, the file has been read
        return true;
    }

    // read the file and create a objects and store in ProjectAlloc class
    bool read_file_and_create_obj(const vector<pair<string, string>> &files)
    {
        try
        {

            // loop through the files
            for (auto &file : files)
            {
                // ignore the allocations file
                if (file.first == "allocations")
                {
                    // return true; exit the loop
                    return true;
                }

                // get file name from the pair
                string file_name = file.first;

                // open the file
                ifstream in_file(file.second);
                // check if the file is open
                bool is_open = check_file_open(in_file, file_name);

                // check if the file is not empty
                bool is_not_empty = check_file_empty(in_file, file_name);

                // check if the file is not empty and is open
                if ((is_open && is_not_empty) == true)
                {
                    // read the file
                    read_file(file_name, in_file);
                    // return true;
                }
            }
        }
        catch (const exception &e)
        {
            cerr << "Error: " << e.what() << endl;
            return false;
        }

        return false;
    }

    // helper function to check if the object is not null

    template <typename T>
    bool is_not_null(const T &obj)
    {
        {
            // check if entity is not null
            return obj != nullptr;
        }
    }

    // function to remove obj from the list
    // template <typename T>
    // void remove_obj(vector<T> &obj_list, const T &obj)
    // {
    //     // check if obj is not null
    //     if (is_not_null(obj))
    //     {
    //         // check if obj is in the list
    //         auto it = get_obj(obj_list, obj);
    //         if (is_not_null(it))
    //         {
    //             // remove obj from the list
    //             obj_list.erase(std::remove(obj_list.begin(), obj_list.end(), obj), obj_list.end());
    //         }
    //     }
    // }

    // function to check if obj is in the list and return the obj
    template <typename T>
    T get_obj(const vector<T> &obj_list, const T &obj)
    {
        // check if obj is not null
        if (is_not_null(obj))
        {
            // check if obj is in the list
            auto it = std::find(obj_list.begin(), obj_list.end(), obj);
            if (it != obj_list.end())
            {
                return *it;
            }
        }
        return nullptr;
    }

    // function to check if obj is in the list and return the obj
    bool check_file_open(ifstream &in_file, const string &file_name)
    {
        // check if the file is open
        if (!in_file.is_open())
        {
            // print the error message
            cerr << "Error: " << file_name << " file is not open!" << endl;
            // throw an exception if the file is not open
            throw invalid_argument("File is not open!");
            return false;
        }
        else
        {
            return true;
        }
    }

    // function to check if the file is empty
    bool check_file_empty(ifstream &in_file, const string &file_name)
    {
        // if no characters to read from the file till the end of the file, file is empty
        if (in_file.peek() == ifstream::traits_type::eof())
        {
            // print the error message
            cerr << "Error: " << file_name << " file is empty!" << endl;
            // throw an exception if the file is empty
            throw invalid_argument("File is empty!");
            return false;
        }

        return true;
    }

    // convert the map into vector
    template <typename T, typename datatype>
    vector<std::pair<datatype, shared_ptr<T>>> convert_map_to_vector(const map<datatype, shared_ptr<T>> &map_list)
    {
        std::vector<std::pair<datatype, shared_ptr<T>>> class_name_vector(map_list.begin(), map_list.end());

        return class_name_vector;
    }

}
// ==--------========= Object class implementation =========--------==//

// object default constructor implementation

Object::Object(any id)
{
    // set person id
    obj_id_ = id;
}

// destructor implementation
Object::~Object() {}

// get person id implementation
any Object::get_obj_id() const { return obj_id_; }

// set person id implementation
void Object::set_obj_id(any id) { obj_id_ = id; }

// handle line implementation
// unique_ptr<Object> Object::handle_line(string class_type, string line)
// {
//     // check type of class
//     if (class_type == "students")
//     {
//         // let student class handle the line
//         return make_unique<Student>(move(Student::handle_line(line)));
//     }
//     if (class_type == "supervisors")
//     {
//         //  let supervisor class handle the line
//         return Supervisor::handle_line(line);
//     }

//     return nullptr;
// }

// print implementation
string Object::print() const
{

    return "";
}

// return the dynamic cast of the child in shared pointer
// template <typename T>
// shared_ptr<T> Object::get_dynamic_cast(const shared_ptr<Object> &obj)
// {
//     // check if the object is not null
//     if (obj != nullptr)
//     {
//         // check if the object is of type T
//         if (is_base_of<T, Object>::value)
//         {
//             // return the dynamic cast of the object
//             return dynamic_pointer_cast<T>(obj);
//         }
//     }
//     return nullptr;
// }
// ====------ Student class implementation ------====//
// student default constructor implementation
Student::Student(string student_id, vector<int> student_project_preferences) : Object(student_id)
{

    // set project preferences
    this->project_preference_ids_ = student_project_preferences;

    // set total number of project preferences
    total_project_preference_count_ = project_preference_ids_.size();

    // set project allocated to nullptr
    project_allocated_ = nullptr;
}

// copy constructor implementation
Student::Student(const Student &student) : Object(student)
{
    // get project preferences
    vector<int> student_project_preferences = student.get_project_preference_ids();

    // copy project preferences
    // check if student has project preferences
    if (student_project_preferences.size() > 0)
        for_each(student_project_preferences.begin(), student_project_preferences.end(), [this](const int &project_id)
                 { add_project_preference_id(project_id); }

        );

    // check if  student allocated in student. copy if allocated
    if (student.is_project_allocated())
    {
        // make duplicate of project allocated
        allocate_project_to_student(make_unique<Project>(*student.get_allocated_project()));
    }
    else
    {
        // set project allocated to nullptr by resetting unique pointer
        project_allocated_.reset();
    }
}

// destructor implementation
Student::~Student() {}

// add project preference implementation
void Student::add_project_preference_id(int project_preferences)
{
    // insert project preference
    project_preference_ids_.push_back(project_preferences);
    // increase total project preferences number by 1
    total_project_preference_count_++;
}

// get project preferences implementation
vector<int> Student::get_project_preference_ids() const { return project_preference_ids_; }

// get total number of project preferences implementations
// int Student::get_total_project_preference_count() const { return total_project_preference_count_; }

// exceeds_project_preferences
// bool Student::has_reached_project_preference_limit() const
// {
//     // only 4 project preferences allowed else return false
//     return total_project_preference_count_ >= 4;
// }

// get project allocated implementation
// get duplicate of project allocated
shared_ptr<Project> Student::get_allocated_project() const
{
    // make duplicate of project allocated
    return make_unique<Project>(*project_allocated_);
}

// set project allocated implementation
void Student::allocate_project_to_student(shared_ptr<Project> project)
{
    // set project allocated
    project_allocated_ = move(project);
}

// add project to student
void Student::add_project_to_student(const int &project_id)
{
    // add project to student
    shared_ptr<Project> project = ProjectAlloc::get_object_by_id<Project, int>(project_id);
    // add project to student
    project_allocated_ = move(project);
}

// get the project allocated
shared_ptr<Project> Student::get_project_allocated() const
{
    // return the project allocated
    return project_allocated_;
}

// get the student allocation score
int Student::get_student_allocation_score() const
{
    return student_allocation_score_;
}

// set the student allocation score
void Student::set_student_allocation_score(int score)
{
    student_allocation_score_ = score;
}

// check if student is allocated implementation
bool Student::is_project_allocated() const
{
    // get the project allocated
    // unique_ptr<Project> project_allocated = get_allocated_project();
    // check if project allocated is not null
    return is_project_allocated_;
}

// update the project allocation state
void Student::update_project_allocation_state(bool status)
{
    // update the project allocation status
    is_project_allocated_ = status;
}
// get the supervisor who supervises the project allocated to the student
// shared_ptr<Supervisor> Student::get_student_supervisor() const
// {

//     // get the supervisor of the project allocated
//     return this->supervisor_;
// }

// set the supervisor who supervises the project allocated to the student
// void Student::set_supervisor_to_student(const any &supervisor_id)
// {
//     // search for the supervisorr by id
//     shared_ptr<Supervisor> supervisor = ProjectAlloc::get_object_by_id<Supervisor, string>(std::any_cast<string>(supervisor_id));

//     // set the supervisor of the project allocated
//     this->supervisor_ = move(supervisor);
// }

void Student::set_supervisor_to_student(const any &supervisor_id)
{
    // search for the supervisorr by id
    shared_ptr<Supervisor> supervisor = ProjectAlloc::get_object_by_id<Supervisor, string>(std::any_cast<string>(supervisor_id));

    // set the supervisor of the project allocated
    this->supervisor_ = move(supervisor);
}

// print implementation
string Student::print() const
{
    // get student preferences
    vector<int> student_project_preferences = get_project_preference_ids();

    // build string stream
    stringstream ss;

    // print student details
    ss << "Student id: " << any_cast<string>(get_obj_id()) << endl;
    // print project preferences
    ss << "Project preferences: " << endl;
    for_each(student_project_preferences.begin(), student_project_preferences.end(), [&ss](const int &project_id)
             { ss << project_id << endl; });

    // check if student is allocated
    if (is_project_allocated())
    {
        // print project allocated
        ss << "Project allocated: " << any_cast<string>(this->get_allocated_project()->get_obj_id()) << endl;
    }
    else
    {
        // print project allocated
        ss << "Project allocated: None" << endl;
    }

    return ss.str();
}

// << operator implementation
ostream &operator<<(ostream &os, const Student &student)
{
    // print student details
    os << student.print();
    return os;
}

// ====------ Project class implementation ------====//

// project default constructor implementation
Project::Project(int project_id, shared_ptr<Supervisor> supervisor_id, int project_multiplicity, string project_title) : Object(project_id)
{
    // set the supervisor for this project
    supervisor_ = move(supervisor_id);
    // also add this project id to the supervisor's proposed projects

    // set project title
    project_title_ = project_title;
    // set project multiplicity
    project_multiplicity_ = project_multiplicity;

    // set project allocated to false
    available_space_in_project_ = project_multiplicity;
}

// copy constructor implementation
Project::Project(const Project &project) : Object(project)
{
    // get project details
    int project_id = any_cast<int>(Object::get_obj_id());
    string project_title = project.get_project_title();
    int project_multiplicity = project.get_project_multiplicity();
    const vector<shared_ptr<Student>> &students_allocated_to_project = project.get_students_allocated();
    const shared_ptr<Supervisor> &project_supervisor = project.get_project_supervisor();

    // copy the available space in project
    available_space_in_project_ = project.get_available_space();

    // set project details
    set_obj_id(project_id);
    set_project_title(project_title);
    set_project_multiplicity(project_multiplicity);

    set_supervisor_to_project(move(project_supervisor));

    // copy students allocated
    for_each(students_allocated_to_project.begin(), students_allocated_to_project.end(), [this](const shared_ptr<Student> &student)
             { alloc_student_to_proj(make_unique<Student>(*student)); });
}

// destructor implementation
Project::~Project() {}

// get project title implementation
string Project::get_project_title() const { return project_title_; }

// set project title implementation
void Project::set_project_title(string project_title) { project_title_ = project_title; }

// get multiplicity implementation
int Project::get_project_multiplicity() const { return project_multiplicity_; }

// set multiplicity implementation
void Project::set_project_multiplicity(int project_multiplicity) { project_multiplicity_ = project_multiplicity; }

// add student allocated implementation
void Project::alloc_student_to_proj(const shared_ptr<Student> &student)
{
    // search the student in the list of students ProjectAlloc
    // shared_ptr<Student> student = ProjectAlloc::get_object_by_id<Student, string>(student_id);
    // add student allocated to project
    this->num_of_students_allocated_.emplace_back(move(student));

    // up date the available space in the project
}

void Project::update_available_space()
{
    // minus the available space by 1
    this->available_space_in_project_--;
}
// set supervisor implementation
void Project::set_supervisor_to_project(const shared_ptr<Supervisor> &allocate_supervisor)
{
    // set supervisor
    supervisor_ = make_unique<Supervisor>(*allocate_supervisor);
}

// get the availability of project implementation
// bool Project::is_project_available_to_allocate() const
// {
//     // check if project is available
//     return is_project_available_;
// }

// set the availability of project implementation
void Project::set_is_project_available_to_allocate(bool is_project_available)
{
    // set project availability
    is_project_available_ = is_project_available;
}

// get available space implementation
int Project::get_available_space() const
{
    // making sure available space is not negative value
    // return available space
    return available_space_in_project_;
}

// get total number of students allocated implementation
int Project::get_total_of_students_allocated_to_project() const
{

    // get student allocated
    const vector<shared_ptr<Student>> &students_allocated_in_project = get_students_allocated();

    int total = 0;

    // check if students_allocated is not empty
    (students_allocated_in_project.size() > 0) ? total = students_allocated_in_project.size() : total;
    // return total number of students allocated
    return total;
}

// get students allocated implementation
const vector<shared_ptr<Student>> &Project::get_students_allocated() const
{
    // return list of students allocated

    return num_of_students_allocated_;
}

// get supervisor implementation
const shared_ptr<Supervisor> &Project::get_project_supervisor() const
{
    // make new unique pointer of supervisor and return it
    return supervisor_;
}

// is project full implementation
bool Project::is_project_alloc_full() const
{
    // get this project multiplicity
    int multiplicity = get_project_multiplicity(); // 3
    // get the total number of students allocated
    int total_students_allocated = get_total_of_students_allocated_to_project();

    bool is_full = false;

    // check if project is full
    (total_students_allocated >= multiplicity) ? is_full = true : is_full;

    return is_full;
}

// is project allocated implementation
bool Project::has_project_allocated_to_supervisor() const
{
    // get supervisor
    const shared_ptr<Supervisor> &project_supervisor = get_project_supervisor();

    // check if supervisor is not null
    return helper::is_not_null(project_supervisor);
}

// print project details implementation
string Project::print() const
{

    // print project details
    stringstream ss;

    // print project details
    ss << "Project id: " << any_cast<int>(get_obj_id()) << endl;
    ss << "Multiplicity: " << project_multiplicity_ << endl;
    ss << "Available space: " << get_available_space() << endl;

    // check if project is full
    if (is_project_alloc_full())
    {
        ss << "Project is full" << endl;
    }
    else
    {
        ss << "Project is not full" << endl;
    }

    // check if project is allocated
    if (has_project_allocated_to_supervisor())
    {
        ss << "Project is allocated to supervsor" << endl;
    }
    else
    {
        ss << "Project is not allocated" << endl;
    }

    // print supervisor details
    ss << "Supervisor: " << any_cast<string>(supervisor_.get()->get_obj_id()) << endl;

    // print students allocated
    ss << "Students allocated: " << endl;
    for_each(num_of_students_allocated_.begin(), num_of_students_allocated_.end(), [&ss](const shared_ptr<Student> &student)
             { ss << "\t" << any_cast<string>(student->get_obj_id()) << endl; });

    return ss.str();
}

// overload stream  operator implementation
ostream &operator<<(ostream &os, const Project &project)
{
    os << project.print();
    return os;
}

// ====------ Supervisor class implementation ------====//
// supervisor default constructor implementation
Supervisor::Supervisor(string id, int supervision_load) : Object(id)
{
    // set supervisor details
    supervision_load_ = supervision_load;
}

// copy constructor implementation
Supervisor::Supervisor(const Supervisor &supervisor) : Object(supervisor)
{

    // get the supervisor id and cast it to string as it is stored as any in base class
    string id = any_cast<string>(supervisor.get_obj_id());
    // get supervision load
    int supervision_load = supervisor.get_supervision_load();
    // get proposed projects
    const vector<shared_ptr<Project>> &projects_proposed = supervisor.get_projects_proposed();

    // get allocated projects
    const vector<shared_ptr<Project>> &projects_supervised = supervisor.get_projects_supervised();

    // set the supervisor_id to new supervisor
    this->set_obj_id(id);

    // set the supervision load to new supervisor
    set_supervision_load(supervision_load);

    // copy projects proposed

    // check if projects_proposed is not empty
    if (projects_proposed.size() > 0)
        for_each(projects_proposed.begin(), projects_proposed.end(), [this](const shared_ptr<Project> &project)
                 { add_project_proposed(std::make_shared<Project>(*project)); });

    // copy projects supervised
    // for_each(projects_supervised.begin(), projects_supervised.end(), [this](const unique_ptr<Project> &project)
    //          { set_supervisor_to_project(std::make_shared<Project>(*project)); });

    // get students supervised
    const vector<shared_ptr<Student>> &students_supervised = supervisor.get_students_supervised();

    // copy students supervised
    // check if students_supervised is not empty
    if (students_supervised.size() > 0)
        for_each(students_supervised.begin(), students_supervised.end(), [this](const shared_ptr<Student> &student)
                 { add_student_supervised(std::make_shared<Student>(*student)); });
}

// destructor implementation
Supervisor::~Supervisor() {}

// add student supervised implementation
void Supervisor::add_student_supervised(shared_ptr<Student> student_supervised)
{
    // add student supervised
    students_supervised_.push_back(move(student_supervised));
}

// add project to supervise implementation
void Supervisor::add_project_to_supervised(const int &project_id)
{
    // add project to supervise
    shared_ptr<Project> project = ProjectAlloc::get_object_by_id<Project, int>(project_id);
    projects_proposed_.push_back(move(project));

    // update project supervised count
    active_num_of_students_supervised_++;
}

// get supervision load implementation
int Supervisor::get_supervision_load() const { return this->supervision_load_; }

// set supervision load implementation
void Supervisor::set_supervision_load(int supervision_load) { this->supervision_load_ = supervision_load; }

// get the project supervised count implementation
int Supervisor::get_current_supervised_student_count() const
{
    return this->active_num_of_students_supervised_;
}

// set the project supervised count implementation
// void Supervisor::set_project_supervised_count(int project_supervised_count)
// {
//     this->active_num_of_students_supervised_ = project_supervised_count;
// }

// get supervisor available implementation
// bool Supervisor::is_supervisor_available() const { return this->is_supervisor_available_to_alloc_; }

// update supervisor availability implementation
void Supervisor::set_is_supervisor_availability(bool is_available)
{
    this->is_supervisor_available_to_alloc_ = is_available;
}

// get the supervisor with modifiable access implementation get_projects_supervised_modifiable
// vector<shared_ptr<Project>> &Supervisor::get_projects_supervised_modifiable()
// {
//     // return list of projects supervised
//     return projects_supervised_;
// }

// get proposed projects implementation
const vector<shared_ptr<Project>> &Supervisor::get_projects_proposed() const
{
    // return list of projects proposed
    return projects_proposed_;
}

// get project supervised implementation
const vector<shared_ptr<Project>> &Supervisor::get_projects_supervised() const
{
    // return list of projects supervised
    return projects_supervised_;
}

// get total number of projects supervised implementation
int Supervisor::get_total_numb_of_projects_supervised() const
{
    // get projects supervised
    const vector<shared_ptr<Project>> &projects_supervised = get_projects_supervised();

    // return total number of projects supervised
    return projects_supervised.size();
}

void Supervisor::add_project_proposed(shared_ptr<Project> project)
{
}

// {
//     // check if project is not null
//     if (helper::is_not_null(project))
//     {
//         // add project to projects proposed
//         projects_proposed_.push_back(move(project));
//     }
// }

// get available space for projest supervised implementation
int Supervisor::get_available_space() const
{
    // get supervision load
    int supervision_load = get_supervision_load();

    // get total number of projects supervised
    int total_numb_of_projects_supervised = get_total_numb_of_projects_supervised();

    // return available space for projects supervised
    return supervision_load - total_numb_of_projects_supervised;
}

// get is overloaded implementation
bool Supervisor::is_supervisor_overloaded() const
{
    // get available space
    int available_space = get_available_space();

    // check if available space is less than 0
    if (available_space < 0)
    {
        // return true
        return true;
    }

    // return false
    return false;
}

// print implementation
string Supervisor::print() const
{
    // get supervisor details
    string id = any_cast<string>(get_obj_id());
    int supervision_load = get_supervision_load();

    // get projects proposed
    const vector<shared_ptr<Project>> &projects_proposed = get_projects_proposed();

    // get projects supervised
    const vector<shared_ptr<Project>> &projects_supervised = get_projects_supervised();

    // get students supervised
    const vector<shared_ptr<Student>> &students_supervised = get_students_supervised();

    // create string stream
    stringstream ss;

    // add supervisor details to string stream
    ss << "Supervisor ID: " << id << endl;
    ss << "Supervision Load: " << supervision_load << endl;

    // add projects proposed to string stream
    // ss << "Projects Proposed: " << endl;
    // for_each(projects_proposed.begin(), projects_proposed.end(), [&ss](const unique_ptr<Project> &project)
    //          { ss << *project << endl; });

    // // add projects supervised to string stream
    // ss << "Projects Supervised: " << endl;
    // for_each(projects_supervised.begin(), projects_supervised.end(), [&ss](const shared_ptr<Project> &project)
    //          { ss << *project << endl; });

    // // add students supervised to string stream
    // ss << "Students Supervised: " << endl;
    // for_each(students_supervised.begin(), students_supervised.end(), [&ss](const shared_ptr<Student> &student)
    //          { ss << *student << endl; });

    // return string stream
    return ss.str();
}

// {
//     // check if student is not null
//     if (helper::is_not_null(student))
//     {
//         // add student to students supervised
//         students_supervised_.push_back(move(student));
//     }
// }

// get students supervised implementation
const vector<shared_ptr<Student>> &Supervisor::get_students_supervised() const
{
    // return list of students supervised
    return students_supervised_;
}

// Supervisor &Supervisor::operator=(Supervisor supervisor)
// {
//     // Swap the contents of this object with the argument
//     std::swap(obj_id_, supervisor.obj_id_);
//     std::swap(supervision_load_, supervisor.supervision_load_);
//     std::swap(projects_proposed_, supervisor.projects_proposed_);

//     // Return a reference to this object
//     return *this;
// }

// operator overloader implementation
ostream &operator<<(ostream &os, const Supervisor &supervisor)
{
    os << supervisor.print();

    return os;
}

// comparing the supervisor by supervisor load in descending order
bool operator<(const Supervisor &supervisor1, const Supervisor &supervisor2)
{
    // get supervision load
    return supervisor1.get_supervision_load() < supervisor2.get_supervision_load();
}

// comparing the supervisor by supervisor id in ascending order
bool operator>(const Supervisor &supervisor1, const Supervisor &supervisor2)
{
    // get supervisor id
    string supervisor_id1 = any_cast<string>(supervisor1.get_obj_id());
    string supervisor_id2 = any_cast<string>(supervisor2.get_obj_id());
    return supervisor_id1 > supervisor_id2;
}

// =====-------------- ProjectAlloc class ------------=====

// // constructor implementation
ProjectAlloc &ProjectAlloc::get_instance()
{
    // create instance of ProjectAlloc
    return project_allocation_system_;
}

// this declaration is required to avoid linker error
ProjectAlloc ProjectAlloc::project_allocation_system_;
map<string, shared_ptr<Supervisor>> ProjectAlloc::supervisors_;
map<string, shared_ptr<Student>> ProjectAlloc::students_;
map<int, shared_ptr<Project>> ProjectAlloc::projects_;
// destructor implementation
ProjectAlloc::~ProjectAlloc()
{
}

template <typename T>
void ProjectAlloc::add_object_to_map(const std::variant<std::string, int> &id, std::shared_ptr<T> obj)
{
    // Check if the object is a Supervisor
    if constexpr (std::is_same_v<T, Supervisor>)
    {
        // checking the type of id
        // Make sure the ID is a string
        if (auto id_string = std::get_if<std::string>(&id))
        {
            // Add the supervisor to the map of supervisors
            supervisors_.emplace(*id_string, std::move(obj));
        }
        else
        {
            // Handle error, id is not a string
            throw std::invalid_argument("Invalid argument: expected string ID for Supervisor");
        }
    }
    // Check if the object is a Student
    else if constexpr (std::is_same_v<T, Student>)
    {

        // Make sure the ID is a string
        if (auto id_string = std::get_if<std::string>(&id))
        {
            // Add the student to the map of students
            students_.emplace(*id_string, std::move(obj));
        }
        else
        {
            // Handle error, id is not a string
            throw std::invalid_argument("Invalid argument: expected string ID for Student");
        }
    }
    // Check if the object is a Project
    else if constexpr (std::is_same_v<T, Project>)
    {
        // Make sure the ID is an int
        if (auto id_int = std::get_if<int>(&id))
        {
            // Add the project to the map of projects
            projects_.emplace(*id_int, std::move(obj));
        }
        else
        {
            // Handle error, id is not an int
            throw std::invalid_argument("Invalid argument: expected int ID for Project");
        }
    }
    else
    {
        // Handle error, object is not a Supervisor, Student or Project
        throw std::invalid_argument("Invalid argument");
    }
}

// get the map list implementation
template <typename T, typename datatype>
std::map<datatype, shared_ptr<T>> ProjectAlloc::get_map_of_objects()
{
    std::map<datatype, shared_ptr<T>> map_of_objects;

    // Check if the object is a Supervisor
    if constexpr (std::is_same<T, Supervisor>::value)
    {
        // Return the map of supervisors
        return supervisors_;
    }
    else if constexpr (std::is_same<T, Student>::value)
    {
        // Return the map of students
        return students_;
    }
    else if constexpr (std::is_same<T, Project>::value)
    {
        // Return the map of projects
        return projects_;
    }
    else
    {
        // Handle error, object is not a Supervisor, Student or Project
        throw std::invalid_argument("Invalid argument");
    }

    return map_of_objects;
}

// // ProjectAlloc search object by id and return duplicate object implementation
template <typename T, typename datatype>
shared_ptr<T> ProjectAlloc::get_object_by_id(const any &obj_id)
{
    try
    {
        // get the map list
        auto map_list = get_map_of_objects<T, datatype>();
        // print the map list

        auto search_obj = find_if(map_list.begin(), map_list.end(), [&obj_id](const auto &element)
                                  { return std::any_cast<datatype>(element.second->get_obj_id()) == std::any_cast<datatype>(obj_id); });

        // check if the object was found
        if (search_obj != map_list.end())
        {
            // create a duplicate object
            // unique_ptr<T> new_obj = make_unique<T>(*search_obj);
            // unique_ptr<T> new_obj = make_unique<T>(*search_obj.second);
            // unique_ptr<T> new_obj = make_unique<T>(*search_obj->second);
            // make a shared pointer to the object
            // shared_ptr<T> new_obj = search_obj->second;
            // return shared_ptr<T>(search_obj->first);
            return search_obj->second;
        }
        else
        {
            stringstream ss;

            // create error message

            ss << obj_id.type().name() << " with id " << any_cast<datatype>(obj_id) << " not found";

            // throw exception object not found
            throw std::runtime_error(ss.str());
        }
    }
    catch (const std::exception &e)
    {
        // catch exception and print error message to console
        cerr << e.what() << endl;
    }

    // return empty object if not found
    return unique_ptr<T>();
}

// sort the object in descending order implementation
template <typename T, typename datatype>
vector<pair<datatype, shared_ptr<T>>> ProjectAlloc::sort_objects_by_id_descending_order()
{
    // // get the map list
    auto map_list = get_map_of_objects<T, datatype>();

    // call map_to_vector conversion function

    auto converted_to_vector = helper::convert_map_to_vector(map_list);

    if (converted_to_vector.empty() != true)
    {
        // sorting the vector in descending order
        std::sort(converted_to_vector.begin(), converted_to_vector.end(),
                  [](const auto &obj_a, const auto &obj_b)
                  {
                      return obj_a.first > obj_b.first;
                  });
    }
    else
    {
        cerr << "Something went wrong after converting map to vector" << endl;
        throw std::runtime_error("Map is empty");
    }

    return converted_to_vector;
}

// explicit template instantiation
template vector<pair<string, shared_ptr<Supervisor>>> ProjectAlloc::sort_objects_by_id_descending_order<Supervisor, string>();
template vector<pair<string, shared_ptr<Student>>> ProjectAlloc::sort_objects_by_id_descending_order<Student, string>();
template vector<pair<int, shared_ptr<Project>>> ProjectAlloc::sort_objects_by_id_descending_order<Project, int>();

// get the vectors of objects
template <typename T, typename datatype>
vector<pair<datatype, shared_ptr<T>>> ProjectAlloc::get_vector_of_objects()
{
    // get the map list
    auto map_list = get_map_of_objects<T, datatype>();

    // convert the map list into a vector
    auto converted_to_vector = helper::convert_map_to_vector(map_list);

    // return the map list
    return converted_to_vector;
}

// explicit template instantiation
template vector<pair<string, shared_ptr<Supervisor>>> ProjectAlloc::get_vector_of_objects<Supervisor, string>();
template vector<pair<string, shared_ptr<Student>>> ProjectAlloc::get_vector_of_objects<Student, string>();
template vector<pair<int, shared_ptr<Project>>> ProjectAlloc::get_vector_of_objects<Project, int>();

// get the vector of students
vector<pair<string, shared_ptr<Student>>> ProjectAlloc::get_vector_of_students()
{
    // return the map list
    return get_vector_of_objects<Student, string>();
}

// get the vector of supervisors
vector<pair<string, shared_ptr<Supervisor>>> ProjectAlloc::get_vector_of_supervisors()
{
    // return the map list
    return get_vector_of_objects<Supervisor, string>();
}

// get the vector of projects
vector<pair<int, shared_ptr<Project>>> ProjectAlloc::get_vector_of_projects()
{
    // return the map list
    return get_vector_of_objects<Project, int>();
}

// methods for sorting the supervisor by supervision load in descending order
// vector<pair<string, shared_ptr<Supervisor>>> ProjectAlloc::sort_supervisors_load_in_desc_ord()
// {
//     // get the map list
//     auto map_list = get_map_of_objects<Supervisor, string>();

//     // convert the map list into a vector
//     auto converted_to_vector = helper::convert_map_to_vector(map_list);

//     if (converted_to_vector.empty() != true)
//     {
//         // sorting the vector in descending order
//         std::sort(converted_to_vector.begin(), converted_to_vector.end(),
//                   [](const auto &obj_a, const auto &obj_b)
//                   {
//                       return obj_a.second->get_supervision_load() > obj_b.second->get_supervision_load();
//                   });
//     }
//     else
//     {
//         cerr << "Something went wrong after converting map to vector" << endl;
//     }

//     return converted_to_vector;
// }

// sort the project by multiplicity in descending order
// vector<pair<int, shared_ptr<Project>>> ProjectAlloc::sort_objects_by_id_descending_order()
// {

//     // get the vector of projects
//     auto vector_of_projects = get_vector_of_projects();

//     if (vector_of_projects.empty() != true)
//     {
//         // sorting the vector in descending order
//         std::sort(vector_of_projects.begin(), vector_of_projects.end(),
//                   [](const auto &obj_a, const auto &obj_b)
//                   {
//                       return obj_a.second->get_project_multiplicity() > obj_b.second->get_project_multiplicity();
//                   });
//     }
//     else
//     {
//         cerr << "Something went wrong after converting map to vector" << endl;
//     }

//     return vector_of_projects;
// }

// get the vector of unallocated students (students that have not been allocated a project)
// vector<pair<string, shared_ptr<Student>>> ProjectAlloc::get_unallocated_students()
// {
//     // Create a vector to store the unallocated students
//     vector<pair<string, shared_ptr<Student>>> unallocated_students;

//     // Get the vector of all students
//     auto students = get_vector_of_students();

//     // Loop through the students and check if they have been allocated a project
//     for (const auto &[student_id, student_obj] : students)
//     {
//         if (!student_obj->is_project_allocated())
//         {
//             // If the student has not been allocated a project, add them to the vector
//             unallocated_students.emplace_back(student_id, student_obj);
//         }
//     }

//     return unallocated_students;
// }

// get allocated students
// vector<pair<string, shared_ptr<Student>>> ProjectAlloc::get_allocated_students()
// {
//     // Create a vector to store the allocated students
//     vector<pair<string, shared_ptr<Student>>> allocated_students;

//     // Get the vector of all students
//     auto students = get_vector_of_students();

//     // Loop through the students and check if they have been allocated a project
//     for (const auto &[student_id, student_obj] : students)
//     {
//         if (student_obj->is_project_allocated())
//         {
//             // If the student has not been allocated a project, add them to the vector
//             allocated_students.emplace_back(student_id, student_obj);
//         }
//     }

//     return allocated_students;
// }

// sort the projects by multiplicity in ascending order
vector<pair<int, shared_ptr<Project>>> ProjectAlloc::sort_projects_by_multiplicity_desc_ord()
{

    // get the vector of projects
    auto vector_of_projects = get_vector_of_projects();

    if (vector_of_projects.empty() != true)
    {
        // sorting the vector in descending order
        std::sort(vector_of_projects.begin(), vector_of_projects.end(),
                  [](const auto &obj_a, const auto &obj_b)
                  {
                      return obj_a.second->get_project_multiplicity() > obj_b.second->get_project_multiplicity();
                  });
    }
    else
    {
        cerr << "Something went wrong after converting map to vector" << endl;
    }

    return vector_of_projects;
}

// sort the student by preferences size in desc order
vector<pair<string, shared_ptr<Student>>> ProjectAlloc::sort_students_by_preferences_size_asec_ord()
{

    // get the vector of students
    auto vector_of_students = get_vector_of_students();

    if (vector_of_students.empty() != true)
    {
        // sorting the vector in descending order
        std::sort(vector_of_students.begin(), vector_of_students.end(),
                  [](const auto &obj_a, const auto &obj_b)
                  {
                      return obj_a.second->get_project_preference_ids().size() < obj_b.second->get_project_preference_ids().size();
                  });
    }
    else
    {
        cerr << "Something went wrong after converting map to vector" << endl;
    }

    return vector_of_students;
}
// reset supervisor availability
void ProjectAlloc::reset_supervisor_availability()
{
    // get the vector of supervisors
    auto vector_of_supervisors = get_vector_of_supervisors();

    // loop through the vector of supervisors and reset their availability
    for (auto &[supervisor_id, supervisor_obj] : vector_of_supervisors)
    {
        supervisor_obj->set_is_supervisor_availability(true);
    }
}

// reset project availability
void ProjectAlloc::reset_project_availability()
{
    // get the vector of projects
    auto vector_of_projects = get_vector_of_projects();

    // loop through the vector of projects and reset their availability
    for (auto &[project_id, project_obj] : vector_of_projects)
    {
        project_obj->set_is_project_available_to_allocate(false);
    }
}

// calculate the total allocation score of student
// int ProjectAlloc::calculate_student_allocation_score()
// {
//     // total allocation score
//     int total_allocation_score = 0;

//     // get the vector of students
//     auto vector_of_students = get_vector_of_students();

//     // loop through the vector of students and calculate their allocation score
//     for (auto &[student_id, student_obj] : vector_of_students)
//     {
//         total_allocation_score += student_obj->get_student_allocation_score();
//     }

//     return total_allocation_score;
// }

// get all the available projects
vector<pair<int, shared_ptr<Project>>> ProjectAlloc::get_available_projects()
{
    // Create a vector to store the available projects
    vector<pair<int, shared_ptr<Project>>> available_projects;

    // Get the vector of all projects
    auto projects = get_vector_of_projects();

    // Loop through the projects and check if they are available
    for (const auto &[project_id, project_obj] : projects)
    {
        if (project_obj->is_project_alloc_full() == false)
        {
            // If the project is available, add it to the vector
            available_projects.emplace_back(project_id, project_obj);
        }
    }

    return available_projects;
}

// export the allocation results to a file (output_file.txt)
bool ProjectAlloc::is_project_capacity_full(const shared_ptr<Project> &project_obj, const shared_ptr<Student> &student_obj)
{

    return (
        // !student_obj->is_project_allocated() &&
        project_obj->get_available_space() <= 0 && project_obj->get_project_multiplicity() > 0 &&
        project_obj->get_project_multiplicity() >= project_obj->get_students_allocated().size());
}
// checks if the supervisor is available to supervise a student
bool ProjectAlloc::is_supervisor_overloaded(const shared_ptr<Supervisor> &supervisor_obj)
{
    return (
        supervisor_obj->get_supervision_load() > 0 &&
        supervisor_obj->get_current_supervised_student_count() >= supervisor_obj->get_supervision_load());
}

// allocate student to project

// void ProjectAlloc::allocate_project_to_student(
//     const int &project_id, const shared_ptr<Project> &project_obj,
//     const shared_ptr<Student> &student_obj, bool &allocated)
// {
//     project_obj->alloc_student_to_proj(student_obj);

//     // // update the avaialble space in the project
//     project_obj->update_available_space();

//     // get the supervisor id
//     auto supervisor_obj = project_obj->get_project_supervisor();

//     supervisor_obj->add_project_to_supervised(project_id);

//     // // Add the project to the student
//     student_obj->add_project_to_student(project_id);

//     // // Set the student's allocation score
//     // int score = 4 - distance(student_proj_pref_list.begin(), find(student_proj_pref_list.begin(), student_proj_pref_list.end(), project_id));
//     // student_obj->set_student_allocation_score(score);

//     // total_score += score;

//     // // Set the student as allocated
//     student_obj->update_project_allocation_state(true);
//     // Stop looping through the preferences
// }

// function for exporting the output into a file
void ProjectAlloc::export_allocation_results(const int &max_score, vector<pair<string, shared_ptr<Student>>> &best_allocation)
{
    // sort the studentId in ascending order
    sort(best_allocation.begin(), best_allocation.end(), [](const auto &a, const auto &b)
         { return a.first < b.first; });

    // get the score of the best allocation
    for (const auto &[student_id, student_obj] : best_allocation)
    {

        if (student_obj->is_project_allocated())
        {
            cout << student_id << " ";
            cout << std::any_cast<int>(student_obj->get_project_allocated()->get_obj_id()) << endl;
        }
        else if (!student_obj->is_project_allocated())
        {
            cout << student_id << " ";
            cout << "0" << endl;
        }
    }
    cout << max_score << endl;

    // export th best allocation to a file
    ofstream best_allocation_file;
    best_allocation_file.open("alloc.txt");
    // std::ifstream input_file("alloc.txt");
    if (best_allocation_file.good())
    {
        std::cout << "File exists, using it as input\n";
    }
    else
    {
        std::cout << "File does not exist, creating it as output\n";
        std::ofstream output_file("alloc.txt");
    }
    for (const auto &[student_id, student_obj] : best_allocation)
    {
        if (student_obj->is_project_allocated())
        {
            best_allocation_file << student_id << " ";
            best_allocation_file << std::any_cast<int>(student_obj->get_project_allocated()->get_obj_id()) << endl;
        }
        else if (!student_obj->is_project_allocated())
        {
            best_allocation_file << student_id << " ";
            best_allocation_file << "0" << endl;
        }
    }

    // export the score of the best allocation to a file
    best_allocation_file << max_score << endl;
}

// <<<<<<<<<<<<<<<<<< ---- start the project allocation -->>>> >>>>>>>>>>>
void ProjectAlloc::start_project_allocation_system()
{

    try
    {

        // Sort the projects in descending order of multiplicity
        vector<pair<int, shared_ptr<Project>>> sorted_by_multiplicity_project = sort_projects_by_multiplicity_desc_ord();

        // get the list of projects
        vector<pair<int, shared_ptr<Project>>> available_projects = get_available_projects();

        // get the students
        vector<pair<string, shared_ptr<Student>>> student_list = get_vector_of_students();

        // get the students sorted by preference list size in desc
        auto sorted_by_pref_list_size_student = sort_students_by_preferences_size_asec_ord();

        // Initialize the maximum score and the best allocation
        int max_score = 0;

        vector<pair<string, shared_ptr<Student>>> best_allocation;

        // list of all the students who are not allocated
        vector<pair<string, shared_ptr<Student>>> not_allocated_students;

        // Reset the availability of the supervisors and projects
        reset_supervisor_availability();
        reset_project_availability();

        // Initialize the total score for this iteration
        int total_score = 0;
        // Try to assign the student to one of their preferred projects
        bool allocated = false;
        // Loop through the sorted students  student_list - >
        for (const auto &[student_id, student_obj] : sorted_by_pref_list_size_student)
        {
            // Get the student's preference list (for first time [3 1 2 4])
            const auto &student_proj_pref_list = student_obj->get_project_preference_ids();

            // Set the student's allocation score to 0
            student_obj->set_student_allocation_score(0);

            // bool for checking if this student has project preference
            bool has_project_preference = false;

            if (student_proj_pref_list.size() > 0)
            {
                // yes it has project_preferences
                has_project_preference = true;

                // student preferences
                for (const auto &project_id : student_proj_pref_list) //
                {

                    // Get the project object
                    auto project_obj = get_object_by_id<Project, int>(project_id); // project id 3 -> project obj project_obj->get_obj_id();

                    if (!is_project_capacity_full(project_obj, student_obj))
                    {
                        // Get the project supervisor id
                        const auto &supervisor_id = project_obj->get_project_supervisor()->get_obj_id();

                        // Get the supervisor object
                        auto supervisor_obj = get_object_by_id<Supervisor, string>(supervisor_id);

                        // Check if the supervisor is available
                        //   supervisor_obj->is_supervisor_available() &&
                        if (!ProjectAlloc::is_supervisor_overloaded(supervisor_obj))
                        {

                            // // Allocate the project to the student
                            project_obj->alloc_student_to_proj(student_obj);

                            // // // update the avaialble space in the project
                            project_obj->update_available_space();

                            supervisor_obj->add_project_to_supervised(project_id);

                            // // // Add the project to the student
                            student_obj->add_project_to_student(project_id);

                            // // Set the student's allocation score
                            int score = 4 - distance(student_proj_pref_list.begin(), find(student_proj_pref_list.begin(), student_proj_pref_list.end(), project_id));
                            student_obj->set_student_allocation_score(score);

                            total_score += score;

                            // // Set the student as allocated
                            student_obj->update_project_allocation_state(true);

                            // allocated = true;
                            // allocate the project to the student
                            // allocate_project_to_student(project_id, project_obj, student_obj, allocated);

                            // set the score of allocation
                            // int score = 4 - distance(student_proj_pref_list.begin(), find(student_proj_pref_list.begin(), student_proj_pref_list.end(), project_id));
                            // student_obj->set_student_allocation_score(score);
                            // count the score
                            // total_score += score;
                            // Stop looping through the preferences
                            break;
                        }
                        else
                        {
                            // supervisor is overloaded
                            continue;
                        }
                    }
                    else
                    {
                        // project is full
                        continue;
                    }
                }
            }

            else
            {
                // no it does not have project preferences
                continue;
            }
        }

        // Shuffle the student list
        // shuffle(student_list.begin(), student_list.end(), std::mt19937{std::random_device{}()});

        // Loop through the student list again and allocate them to a project
        for (const auto &[student_id, student_obj] : sorted_by_pref_list_size_student)
        {
            // student has no preference
            bool has_project_preference = false;

            // cout << "Student " << not_allocated_students.size() << " was not allocated to any of their preferred projects" << endl;
            // get the student who hasn't been allocated
            if (!student_obj->is_project_allocated())
            {
                for (const auto &[project_id, project_obj] : available_projects)
                {
                    // check if the project is available and has space
                    if (!is_project_capacity_full(project_obj, student_obj))
                    {
                        // get the student id
                        const auto &student_id = student_obj->get_obj_id();
                        // Get the project supervisor id
                        const auto &supervisor_id = project_obj->get_project_supervisor()->get_obj_id();

                        // Get the supervisor object
                        auto supervisor_obj = get_object_by_id<Supervisor, string>(supervisor_id);

                        // Check if the supervisor is available
                        if (!ProjectAlloc::is_supervisor_overloaded(supervisor_obj))
                        {
                            // // Allocate the project to the student
                            project_obj->alloc_student_to_proj(student_obj);

                            // // // update the avaialble space in the project
                            project_obj->update_available_space();

                            supervisor_obj->add_project_to_supervised(project_id);

                            // // // Add the project to the student
                            student_obj->add_project_to_student(project_id);

                            // Set the student's allocation score
                            student_obj->set_student_allocation_score(0);

                            // Set the student as allocated
                            student_obj->update_project_allocation_state(true);

                            // allocate_project_to_student(project_id, project_obj, student_obj, allocated);

                            // Stop looping through the preferences
                            break;
                        }
                    }
                }
            }
        }
        // efficent way to shuffle the student list
        if (total_score > max_score)
        {
            // Update the maximum score and the best allocation
            max_score = total_score;
            if (best_allocation.size() > 0)
                best_allocation.clear();
            best_allocation = get_vector_of_objects<Student, string>();
        }

        // export then result
        export_allocation_results(max_score, best_allocation);
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << endl;
    }
}

// // ====--------- FactoryObject Implementation ---------====
// // create object implementation

bool ObjectFactory::create_object(const string &class_type, const string &line)
{
    // strip the words from the line
    istringstream iss(line);

    if (class_type == "supervisors")
    {
        string supervisor_id;
        // get the second word
        int supervision_load;

        iss >> supervisor_id >> supervision_load;

        // create a new supervisor object
        // std::shared_ptr<Supervisor> supervisor = std::make_shared<Supervisor>(Supervisor(supervisor_id, supervision_load));
        std::shared_ptr<Supervisor> supervisor = std::allocate_shared<Supervisor>(std::allocator<Supervisor>(), supervisor_id, supervision_load);
        // add the supervisor to the supervisors map in ProjectAlloc

        // ProjectAlloc::get_instance().add_object_to_map_list<Supervisor>("supervisors", supervisor_id, move(std::make_shared<Supervisor>(Supervisor(supervisor_id, supervision_load))));
        ProjectAlloc::get_instance().add_object_to_map<Supervisor>(supervisor_id, move(supervisor));

        // yes, created object is passed to the map list
        return true;
    }
    else if (class_type == "students")
    {
        // get the first word
        string student_id;

        vector<int> choices;

        iss >> student_id;

        // get the choices from the line and add them to the choices {1, 2, 3, 4, 5}
        // std::istream_iterator<int> start(iss), end;
        // read the numbers on by one, copy it and then insert it into the choices

        // copy the elements from the stream to set it in backward order
        // std::copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(), std::back_inserter(choices));
        int choice;
        while (iss >> choice)
        {
            choices.push_back(choice);
        }

        // create a shared pointer to the student object
        // shared_ptr<Student> student = std::make_shared<Student>(Student(student_id, choices));
        std::shared_ptr<Student> student = std::allocate_shared<Student>(std::allocator<Student>(), student_id, choices);

        // get the student id from the student object
        ProjectAlloc::get_instance().add_object_to_map<Student>(student_id, move(student));

        // print out the student list_
        // yes, created object is passed to the map list
        return true;
    }
    else if (class_type == "projects")
    {
        // project id
        int project_id;
        // get the first word and set it to project id
        iss >> project_id;

        // supervisor id
        string supervisor_id;
        // get the second word and set it to supervisor id
        iss >> supervisor_id;

        // search supervisor in supervisors map in ProjectAlloc
        auto supervisor_obj = ProjectAlloc::get_object_by_id<Supervisor, string>(supervisor_id);

        // project multiplicity
        int project_multiplicity;
        // get the third word and set it to project multiplicity
        iss >> project_multiplicity;

        string project_title;
        // get rest of the line and set it to project title
        getline(iss, project_title);

        // try to add the project to the projects map in ProjectAlloc
        //    =================== ((this one invokes the default constructor and copy constructor too) (v1 -version 1)==============

        ProjectAlloc::get_instance().add_object_to_map<Project>(project_id, std::make_shared<Project>(Project(project_id, supervisor_obj, project_multiplicity, project_title)));

        // =======================================================================================================

        // creating the Project object without invoking the copy constructor
        //    =================== ((this only invoke the default constructor) (v2- version 2) ==============
        // creating the Project object by using this std::allocate_shared (it best choice for me use it as default constructor is protected)
        // std::shared_ptr<Project> project = std::make_shared<Project>(project_id, supervisor_obj, project_multiplicity, project_title);

        // add the project to the projects map in ProjectAlloc
        // ProjectAlloc::get_instance().add_object_to_map<Project>(project_id, move(project));

        // =======================================================================================================

        // yes, created object is passed to the map list
        return true;
    }
    // if the class type is not valid
    else
    {

        cerr << "Invalid class type " << class_type << endl;
        return false;
    }
    return false;
}

// ObjectFactory cast object unique_ptr to share_ptr implementation
// template <typename T>
// std::shared_ptr<T> ObjectFactory::cast_object_unique_ptr_to_share_ptr(const std::shared_ptr<T> &obj)
// {

//     // Move the unique_ptr to a shared_ptr
//     // auto shared_obj = std::make_shared<Object>(std::move(obj));
//     // Cast the shared_ptr to a shared_ptr<T>
//     return std::dynamic_pointer_cast<T>(obj);
// }
