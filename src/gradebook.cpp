#include <string>
#include "gradebook.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>


// ------------------ Helpers ------------------
Student* Gradebook::find_student_by_id(const std::string& id) {
    auto it = students.find(id);
    if (it != students.end()) return &it->second;
    return nullptr;
}

Assignment* Gradebook::find_assignment(const std::string& name) {
    for (auto& a : assignments) {
        if (a.name == name) return &a;
    }
    return nullptr;
}

// ------------------ Public Methods ------------------
void Gradebook::add_student(const std::string& full_name, const std::string& id) {
    std::istringstream iss(full_name);
    std::string first, last;
    iss >> first >> last;

    Student s{first, last, id, {}};

    for (const auto& a : assignments) {
        s.grades[a.name] = std::nullopt;
    }

    students[id] = s;
}

void Gradebook::add_assignment(const std::string& name, short total_points) {
    assignments.push_back({name, total_points});
    for (auto& [id, s] : students) {
        s.grades[name] = std::nullopt;
    }
}

void Gradebook::enter_grade(const std::string& student_id,
                            const std::string& assignment_name,
                            short grade) {
    Student* student = find_student_by_id(student_id);
    Assignment* assignment = find_assignment(assignment_name);

    if (student && assignment) {
        student->grades[assignment_name] = grade;
    }
}

// ------------------ Full Report ------------------
std::string Gradebook::report() const {
    // Store all rows first
    struct Row { std::string last, first, id;
                 std::vector<std::string> grades;
                 double average;
    };
    std::vector<Row> rows;

    for (const auto& [id, s] : students) {
        Row r;
        r.last = s.last_name;
        r.first = s.first_name;
        r.id = s.id;

        double total_earned = 0;
        double total_possible = 0;
        for (const auto& a : assignments) {
            auto it = s.grades.find(a.name);
            if (it != s.grades.end() && it->second.has_value()) {
                r.grades.push_back(std::to_string(it->second.value()));
                total_earned += it->second.value();
                total_possible += a.total_points;
            } else {
                r.grades.push_back("none");
            }
        }

        r.average = (total_possible > 0) ? (total_earned / total_possible * 100.0) : 0.0;
        rows.push_back(r);
    }

    // Build output string once
    std::ostringstream out;
    out << "=== GRADEBOOK REPORT ===" << std::endl;

    out << "Last_Name,First_Name,Student_Id";
    for (const auto& a : assignments) out << "," << a.name;
    out << ",Average" << std::endl;

    for (const auto& r : rows) {
        out << r.last << "," << r.first << "," << r.id;
        for (const auto& g : r.grades) out << "," << g;
        out << "," << std::fixed << std::setprecision(3) << r.average << std::endl;
    }

    return out.str();
}

// ------------------ Assignment Report ------------------
std::string Gradebook::assignment_report(const std::string& assignment_name) const {
    // Store data first
    struct Row {
        std::string last, first, id, score;
    };

    std::vector<Row> rows;
    double total = 0;
    int count = 0;
    int max_points = 0;

    for (const auto& a : assignments)
        if (a.name == assignment_name) {
            max_points = a.total_points;
        }

    for (const auto& [id, s] : students) {
        Row r;
        r.last = s.last_name;
        r.first = s.first_name;
        r.id = s.id;

        auto it = s.grades.find(assignment_name);
        if (it != s.grades.end() && it->second.has_value()) {
            r.score = std::to_string(it->second.value());
            total += it->second.value();
            count++;
        } else {
            r.score = "none";
        }

        rows.push_back(r);
    }

    // Build output string once
    std::ostringstream out;
    out << "=== ASSIGNMENT REPORT: " << assignment_name << " ===" << std::endl;

    out << "Last_Name,First_Name,Student_Id,Score" << std::endl;
    for (const auto& r : rows) {
        out << r.last << "," << r.first << "," << r.id << "," << r.score << std::endl;
    }

    out << std::endl << "Average score: ";
    if (count > 0) {
        out << (total / count) << " / " << max_points;
    }
    else {
        out << "0 / " << max_points;
    }

    return out.str();
}

void Gradebook::load_from_file(const std::string& filename) {
    std::ifstream in;
    in.open(filename);

    if (!in.is_open()) {
        return;
    }

    std::string line;

    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string type;
        std::getline(ss, type, ',');

        if (type == "STUDENT") {
            std::string first, last, id;
            std::getline(ss, first, ',');
            std::getline(ss, last, ',');
            std::getline(ss, id, ',');

            add_student(first + " " + last, id);
        }
        else if (type == "ASSIGNMENT") {
            std::string name, points_str;
            std::getline(ss, name, ',');
            std::getline(ss, points_str, ',');

            add_assignment(name, std::stoi(points_str));
        }
        else if (type == "GRADE") {
            std::string id, assignment, grade_str;
            std::getline(ss, id, ',');
            std::getline(ss, assignment, ',');
            std::getline(ss, grade_str, ',');

            enter_grade(id, assignment, std::stoi(grade_str));
        }
    }
    in.close();
}

void Gradebook::save_to_file(const std::string& filename) const {
    std::ofstream out;
    out.open(filename);

    if (!out.is_open()) {
        return;
    }

    //Students
    for (const auto& [id, s] : students) {
        out << "STUDENT," << s.first_name << "," << s.last_name << ","
            << s.id << std::endl;
    }

    out << std::endl;

    //Assignments
    for (const auto& a : assignments) {
        out << "ASSIGNMENT," << a.name << "," << a.total_points << std::endl;
    }

    out << std::endl;

    //Grades
    for (const auto& [id, s] : students) {
        for (const auto& [assignment, grade] : s.grades) {
            if (grade.has_value()) {
                out << "GRADE," << s.id << "," << assignment << "," << grade.value() << std::endl;
            }
        }
    }

    out.close();
}