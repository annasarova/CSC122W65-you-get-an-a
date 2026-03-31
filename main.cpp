#include <iostream>
#include <fstream>
#include <limits>
#include "src/gradebook.hpp"

// ---------- Helper ----------
void output_report(const std::string& report) {
    short choice;

    std::cout << "\n1. Display in console\n";
    std::cout << "2. Save to file\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        std::cout << report << std::endl;
    }
    else if (choice == 2) {
        std::string filename;
        std::cout << "Enter filename: ";
        std::getline(std::cin, filename);

        std::ofstream out;
        out.open(filename);

        if (out.is_open()) {
            out << report;
            out.close();
            std::cout << "Report saved.\n";
        } else {
            std::cout << "Error opening file.\n";
        }
    }
}

// ---------- Main ----------
int main() {
    Gradebook gradebook;
    std::string file = "gradebook.txt";

    gradebook.load_from_file(file);

    short choice = 0;

    while (choice != 6) {
        std::cout << "\n=== MENU ===\n";
        std::cout << "1. Gradebook Report\n";
        std::cout << "2. Assignment Report\n";
        std::cout << "3. Add Student\n";
        std::cout << "4. Add Assignment\n";
        std::cout << "5. Enter Grade\n";
        std::cout << "6. Exit\n";
        std::cout << "Choice: ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore();

        if (choice == 1) {
            output_report(gradebook.report());
        }
        else if (choice == 2) {
            std::string name;
            std::cout << "Assignment name: ";
            std::getline(std::cin, name);

            output_report(gradebook.assignment_report(name));
        }
        else if (choice == 3) {
            std::string name, id;

            std::cout << "Full name: ";
            std::getline(std::cin, name);

            std::cout << "Student ID: ";
            std::getline(std::cin, id);

            gradebook.add_student(name, id);
        }
        else if (choice == 4) {
            std::string name;
            short pts;

            std::cout << "Assignment name: ";
            std::getline(std::cin, name);

            std::cout << "Points: ";
            std::cin >> pts;
            std::cin.ignore();

            gradebook.add_assignment(name, pts);
        }
        else if (choice == 5) {
            std::string id, name;
            short grade;

            std::cout << "Student ID: ";
            std::getline(std::cin, id);

            std::cout << "Assignment: ";
            std::getline(std::cin, name);

            std::cout << "Grade: ";
            std::cin >> grade;
            std::cin.ignore();

            gradebook.enter_grade(id, name, grade);
        }
        else if (choice == 6) {
            gradebook.save_to_file(file);
            std::cout << "Saved. Goodbye.\n";
        }
    }

    return 0;
}
