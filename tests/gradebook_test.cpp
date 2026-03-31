#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_constructor.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "../src/gradebook.hpp"

// ---------- Student Tests ----------
TEST_CASE("add_student adds a student to report") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");

    std::string r = g.report();

    REQUIRE(r.find("Bobberson") != std::string::npos);
}

TEST_CASE("multiple students appear in report") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_student("Sam Sammerson", "DEF456");

    std::string r = g.report();

    REQUIRE(r.find("Bobberson") != std::string::npos);
    REQUIRE(r.find("Sammerson") != std::string::npos);
}

// ---------- Assignment Tests ----------
TEST_CASE("assignment appears in header") {
    Gradebook g;

    g.add_assignment("Quiz 1", 100);

    std::string r = g.report();

    REQUIRE(r.find("Quiz 1") != std::string::npos);
}

// ---------- Grade Entry Tests ----------
TEST_CASE("enter_grade stores correct value") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_assignment("Quiz 1", 100);

    g.enter_grade("ABC123", "Quiz 1", 88);

    std::string r = g.report();

    REQUIRE(r.find("88") != std::string::npos);
}

TEST_CASE("missing grade shows none") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_assignment("Quiz 1", 100);

    std::string r = g.report();

    REQUIRE(r.find("none") != std::string::npos);
}

// ---------- Average Tests ----------
TEST_CASE("average is calculated correctly for one grade") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_assignment("Quiz 1", 100);

    g.enter_grade("ABC123", "Quiz 1", 50);

    std::string r = g.report();

    REQUIRE(r.find("50.000") != std::string::npos);
}

TEST_CASE("average ignores missing grades") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_assignment("Quiz 1", 100);
    g.add_assignment("Lab 1", 50);

    g.enter_grade("ABC123", "Quiz 1", 100);
    // Lab 1 missing

    std::string r = g.report();

    REQUIRE(r.find("100.000") != std::string::npos);
}

// ---------- Assignment Report Tests ----------
TEST_CASE("assignment_report shows student scores") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_assignment("Quiz 1", 100);

    g.enter_grade("ABC123", "Quiz 1", 77);

    std::string r = g.assignment_report("Quiz 1");

    REQUIRE(r.find("77") != std::string::npos);
}

TEST_CASE("assignment_report shows none for missing grade") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_assignment("Quiz 1", 100);

    std::string r = g.assignment_report("Quiz 1");

    REQUIRE(r.find("none") != std::string::npos);
}

TEST_CASE("assignment_report calculates average") {
    Gradebook g;

    g.add_student("Bob Bobberson", "ABC123");
    g.add_student("Sam Sammerson", "DEF456");

    g.add_assignment("Quiz 1", 100);

    g.enter_grade("ABC123", "Quiz 1", 80);
    g.enter_grade("DEF456", "Quiz 1", 100);

    std::string r = g.assignment_report("Quiz 1");

    REQUIRE(r.find("90") != std::string::npos); // average = 90
}