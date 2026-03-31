#include <string>
#include <vector>
#include <map>
#include <optional>

struct  Assignment {
    std::string name;
    short total_points;
};

struct Student {
    std::string first_name;
    std::string last_name;
    std::string id;
    //assignment name -> grade (optional if not entered)
    std::map<std::string,std::optional<short>> grades;
};
class Gradebook {
private:
    std::map<std::string, Student> students; // key: student ID
    std::vector<Assignment> assignments;

    Student* find_student_by_id(const std::string& id);
    Assignment* find_assignment(const std::string& name);

public:
    // core features
    void add_student(const std::string& full_name, const std::string& id);
    void add_assignment(const std::string& name, short total_points);
    void enter_grade(const std::string& student_id,
                     const std::string& assignment_name, short grade);

    // reports
    std::string report() const;
    std::string assignment_report(const std::string& assignment_name) const;

    //file I/O
    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename) const;
};
