//-------
// @file allocation.h
// @brief Header file for allocation.cpp
// @details Contains the declaration of the classes and functions.
// @design_pattern - Mixture of Factory and Singleton design pattern (Singleton for ProjectAlloc class - only one instance will exist, Factory for ObjectFactory class - will create objects of Object type class and its derived classes)
/// @design_pattern_inspiration - https://www.baeldung.com/creational-design-patterns

#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <numeric>
#include <fstream>
#include <stdexcept>
#include <any>
#include <unordered_set>
#include <map>
#include <variant>
#include <sstream>
#include <fstream>
#include <any>

using std::any;
using std::any_cast;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::make_shared;
using std::make_unique;
using std::map;
using std::minus;
using std::ostream;
using std::pair;
using std::remove;
using std::set;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::unordered_set;
using std::vector;
using std::weak_ptr;

// forward declaration

class Project;
class Supervisor;
class Student;
class ObjectFactory;
class ProjectAlloc;

/// ========------- Helper functions (namespace )-------========///

namespace helper
{
    // helper function for checking number of file input
    bool check_num_of_files_input(int argc, char *argv[], stringstream &ss);
    // function for checking object is not null
    template <typename T>
    bool is_not_null(const T &obj);

    // function for removing obj from the list
    template <typename T>
    void remove_obj(vector<T> &obj_list, const T &obj);

    // function to check if the file is open
    bool check_file_open(ifstream &in_file, const string &file_name);

    // function to check if the file is empty
    bool check_file_empty(ifstream &in_file, const string &file_name);

    // helper function for read file
    bool read_file(const string &file_name, ifstream &in_file);

    // helper function for read file and create objects
    bool read_file_and_create_obj(const vector<pair<string, string>> &files);

    // convert the map into vector
    template <typename T, typename datatype>
    vector<std::pair<datatype, shared_ptr<T>>> convert_map_to_vector(const map<datatype, shared_ptr<T>> &map_obj);
}

// ------============= Object class =============------//

// making this class as a template class to allow for different types of id (int, string)

class Object
{
private:
    // person id
    any obj_id_;

protected:
    // default constructor
    Object(any obj_id_);

public:
    virtual ~Object();

    // get id
    any get_obj_id() const;

    // set id
    void set_obj_id(any id);

    // print
    virtual string
    print() const;

    // get the child obj casted to the child class
    // template <typename T>
    // shared_ptr<T> get_dynamic_cast(const shared_ptr<Object> &obj);

    // friend class
    friend class Project;
    friend class Supervisor;
    friend class Student;
    friend class ObjectFactory;
    friend class ProjectAlloc;
};

// =========-------- Student class --------========//
class Student : public Object
{
private:
    // list of project preferences
    // using unordered_set data structure to avoid duplicates and to allow store preference in descending order (4, 3, 2, 1)
    vector<int> project_preference_ids_;

    // project allocated
    shared_ptr<Project> project_allocated_;

    // student supervised by supervisor
    shared_ptr<Supervisor> supervisor_;

    // total number of projects preferences . default is 0
    int total_project_preference_count_ = 0;

    // project allocation state
    bool is_project_allocated_ = false;

    // student allocation score
    int student_allocation_score_ = 0;

protected:
    // student default constructor

public:
    Student(string id, vector<int> project_preferences);
    // copy constructor
    Student(const Student &student);

    // add project preference
    void add_project_preference_id(int project_preferences);

    // get project preferences
    vector<int> get_project_preference_ids() const;

    // get total number of project preferences
    int get_total_project_preference_count() const;

    // check if exceeds project preferences
    bool has_reached_project_preference_limit() const;

    // get project allocated
    shared_ptr<Project> get_allocated_project() const;

    // set project allocated§
    void allocate_project_to_student(shared_ptr<Project> project);

    // add project to student
    void add_project_to_student(const int &project_id);

    shared_ptr<Project> get_project_allocated() const;

    // check if student is allocated
    bool is_project_allocated() const;

    // set the student allocation score
    void set_student_allocation_score(int score);

    // get the student allocation score
    int get_student_allocation_score() const;

    // update the project allocation state
    void update_project_allocation_state(bool status);
    // get student supervisor
    shared_ptr<Supervisor> get_student_supervisor() const;

    // set student supervisor
    void set_supervisor_to_student(const any &supervisor_id);
    // print
    string print() const override;

    // destructor
    ~Student();

    // assignment operator
    // Student &operator=(const Student &student);

    // overloader of the object in stream
    friend ostream &operator<<(ostream &out, const Student &student);

    // comparing the student in ascending order
    friend bool operator<(const Student &student1, const Student &student2);

    // comparing the student in descending order
    friend bool operator>(const Student &student1, const Student &student2);

    // friend class
    friend class Project;
    friend class Supervisor;
    friend class ProjectAlloc;
    friend class ObjectFactory;
};

// =========-------- Project class --------========//
class Project : public Object
{
private:
    // project title
    string project_title_;

    // project multiplicity
    int project_multiplicity_;

    // available slots
    int available_space_in_project_;

    // list of students allocated to the project
    vector<shared_ptr<Student>> num_of_students_allocated_;

    // set the project supervisor
    shared_ptr<Supervisor> supervisor_;

    bool is_project_available_ = true;

protected:
    // default constructor

public:
    Project(int project_id, shared_ptr<Supervisor> supervisor_id, int project_multiplicity, string project_title);
    // copy constructor
    Project(const Project &project);

    ~Project();

    // get title
    string get_project_title() const;

    // set title
    void set_project_title(string title);

    // get multiplicity
    int get_project_multiplicity() const;

    // set multiplicity
    void set_project_multiplicity(int multiplicity);

    void update_available_space();
    // add student allocated
    void alloc_student_to_proj(const shared_ptr<Student> &student);

    // get students allocated
    const vector<shared_ptr<Student>> &get_students_allocated() const;

    // get supervisor
    const shared_ptr<Supervisor> &get_project_supervisor() const;

    // set supervisor
    void set_supervisor_to_project(const shared_ptr<Supervisor> &supervisor);

    // check if project is available
    bool is_project_available_to_allocate() const;

    // set project available
    void set_is_project_available_to_allocate(bool is_project_available);
    // get available space
    int get_available_space() const;

    // check if project is full
    bool is_project_alloc_full() const;

    // check if project is allocated
    bool has_project_allocated_to_supervisor() const;

    // get total number of students allocated
    int get_total_of_students_allocated_to_project() const;

    // print
    string print() const;

    // assignment operator
    // Project &operator=(const Project &project);

    // overloader of the object in stream
    friend ostream &operator<<(ostream &out, const Project &project);

    // comparison operator for sorting project in ascending order
    friend bool operator<(const Project &project1, const Project &project2);

    // comparison operator for sorting project in ascending order
    friend bool operator>(const Project &project1, const Project &project2);

    // friend class
    friend class Student;
    friend class Supervisor;
    friend class ProjectAlloc;
    friend class ObjectFactory;

    // friend class
};

// =========-------- Supervisor class --------========//
class Supervisor : public Object
{
private:
    // supervisor load (it is student)
    int supervision_load_;

    // total number of projects supervised
    // int total_projects_supervised_;

    // total number of students supervised
    // int active_num_of_students_supervised_; // it is not yet utilized

    bool is_supervisor_available_to_alloc_ = true;

    // list of projects proposed
    vector<shared_ptr<Project>> projects_proposed_;

    // list of projects supervised (stored as weak_ptr to avoid circular reference)
    vector<shared_ptr<Project>> projects_supervised_;

    // list of students supervised (stored as weak_ptr to avoid circular reference)
    vector<shared_ptr<Student>> students_supervised_;

    int active_num_of_students_supervised_ = 0;

protected:
    // default constructor

public:
    Supervisor(string id, int supervision_load);
    // copy constructor
    Supervisor(const Supervisor &supervisor);

    ~Supervisor();

    // get supervision load
    int get_supervision_load() const;

    // set supervision load
    void set_supervision_load(int supervision_load);

    // get the project supervised count
    int get_current_supervised_student_count() const;

    // set the project supervised count
    void set_project_supervised_count(int count);

    // is supervisor available to allocate
    bool is_supervisor_available() const;

    // set supervisor available to allocate
    void set_is_supervisor_availability(bool is_available);
    // add student supervised
    void add_student_supervised(shared_ptr<Student> student);

    // add project supervised
    void add_project_to_supervised(const int &project_id);

    // get students supervised
    const vector<shared_ptr<Student>> &get_students_supervised() const;

    // get projects proposed
    const vector<shared_ptr<Project>> &get_projects_proposed() const;

    // 471
    // get projects supervised
    const vector<shared_ptr<Project>> &get_projects_supervised() const;

    vector<shared_ptr<Project>> &get_projects_supervised_modifiable();

    // get total number of projects supervised
    int get_total_numb_of_projects_supervised() const;

    // add project proposed
    void add_project_proposed(shared_ptr<Project> project);

    // get available space for projects supervised
    int get_available_space() const;

    // check if supervisor is overloaded
    bool is_supervisor_overloaded() const;

    // print
    string print() const override;

    // assignment operator
    // Supervisor &operator=(const Supervisor &supervisor);

    // overloader of the object in stream
    friend ostream &operator<<(ostream &out, const Supervisor &supervisor);

    // comparing the supervisor by supervision load in descending order
    friend bool operator<(const Supervisor &supervisor1, const Supervisor &supervisor2);

    // comparing the supervisor by supervisor id in ascending order
    friend bool operator>(const Supervisor &supervisor1, const Supervisor &supervisor2);

    // friend class
    friend class ObjectFactory;

    friend class ProjectAlloc;
};

// ==========================----------------- ProJectAlloc class ----------------=========================//
class ProjectAlloc
{
private:
    // storing three class objects in a map : <key> is the id of the object and <value> is the shared_ptr to the object
    // making the map static so only one instance of this list will be created  (means only one list available in the memory)
    static map<string, shared_ptr<Supervisor>> supervisors_;
    static map<string, shared_ptr<Student>> students_;
    static map<int, shared_ptr<Project>> projects_;
    // preventing default constructor to call by any other class (i am using the singleton pattern - so it will create only one instance of this during whole program)
    ProjectAlloc(){};

    // only one instance of this class
    static ProjectAlloc project_allocation_system_;

public:
    ~ProjectAlloc();

    // get the instance of the singleton
    static ProjectAlloc &get_instance();

    // a function that start the project allocation system
    void start_project_allocation_system();

    // search object by id
    template <typename T, typename datatype>
    static shared_ptr<T> get_object_by_id(const any &obj_id);

    // get vector list of objects
    template <typename T, typename datatype>
    static vector<pair<datatype, shared_ptr<T>>> get_vector_of_objects();

    static vector<pair<string, shared_ptr<Supervisor>>> get_vector_of_supervisors();
    static vector<pair<string, shared_ptr<Student>>> get_vector_of_students();
    static vector<pair<int, shared_ptr<Project>>> get_vector_of_projects();

    // get the map of objects dynamically
    template <typename T, typename datatype>
    static map<datatype, shared_ptr<T>> get_map_of_objects();

    // // add object to the map
    template <typename T>
    static void add_object_to_map(const std::variant<std::string, int> &id, std::shared_ptr<T> obj);

    // short the object by descending order
    template <typename T, typename datatype>
    static vector<pair<datatype, shared_ptr<T>>> sort_objects_by_id_descending_order();

    // sort the supervisor by supervision load in descending order
    static vector<pair<string, shared_ptr<Supervisor>>> sort_supervisors_load_in_desc_ord();

    // sort the project by project multiplicity in descending order
    static vector<pair<int, shared_ptr<Project>>> sort_objects_by_id_descending_order();

    // get the unallocated students
    static vector<pair<string, shared_ptr<Student>>> get_unallocated_students();

    // get the allocated students
    static vector<pair<string, shared_ptr<Student>>> get_allocated_students();

    // short the project by project multiplicity in descending order
    static vector<pair<int, shared_ptr<Project>>> sort_projects_by_multiplicity_desc_ord();

    // sort the student by preferences size in descend order
    static vector<pair<string, shared_ptr<Student>>> sort_students_by_preferences_size_asec_ord();

    // reset the supervisor availability
    static void reset_supervisor_availability();

    // reset the project availability
    static void reset_project_availability();

    // calculate the student allocation score
    static int calculate_student_allocation_score();

    // static method
    static bool is_project_capacity_full(const shared_ptr<Project> &project_obj, const shared_ptr<Student> &student_obj);

    // static method for checking if the supervisor is overloaded
    static bool is_supervisor_overloaded(const shared_ptr<Supervisor> &supervisor);

    // static method for getting all the available projects
    static vector<pair<int, shared_ptr<Project>>> get_available_projects();

    // static method for getting all the available supervisors
    // static vector<pair<string, shared_ptr<Supervisor>>> get_available_supervisors();

    // allocate the student to the project
    // static void allocate_project_to_student(const int &project_id, const shared_ptr<Project> &project_obj,
    //                                        const shared_ptr<Student> &student_obj,
    //                                         bool &allocated);

    // static method for exporting the data to the file
    static void export_allocation_results(const int &max_score, vector<pair<string, shared_ptr<Student>>> &best_allocation);
};

// ========--------- Object Factory class ---------========//

class ObjectFactory
{
public:
    static bool create_object(const string &class_type, const string &line);

    // cast the object to a shared_ptr<T>
    template <typename T>
    static std::shared_ptr<T> cast_object_unique_ptr_to_share_ptr(const std::shared_ptr<T> &obj);

    // friend class
    friend class ProjectAlloc;
};

#endif
