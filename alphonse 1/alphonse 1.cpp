#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <random>
#include <cstring> // For strcpy (C-style string manipulation)

// Structure with 4 fields
struct Student {
    char rose[50];        // 50 bytes
    char surname[50];     // 50 bytes
    int age;              // 4 bytes
    float grade;          // 4 bytes
};

int main() {
    // Random generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> ageDist(17, 45);
    std::uniform_real_distribution<> gradeDist(5, 25);

    // Lists of rose and surname for random generation
    std::vector<std::string> rose = { "Junior", "Johnson", "Jean", "Louis", "Jess",
                                       "Garcia", "Milla", "David", "Rodrigue", "Martin" };
    std::vector<std::string> surnames = { "James", "Maria", "John", "Patrice", "Robert",
                                          "Jennie", "Michael", "Linda", "Will", "Eliza" };

    std::uniform_int_distribution<> indexDist(0, rose.size() - 1);

    // 1. WRITING TO BINARY FILE
    std::cout << "=== WRITING DATA ===" << std::endl;

    std::ofstream outputFile("students.bin", std::ios::binary);

    if (!outputFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        Student s;

        // Fill fields with random values
        std::string name = rose[indexDist(gen)];
        std::string surname = surnames[indexDist(gen)];

        // Copy strings to char arrays (C-style)
        strcpy(s.rose, name.c_str());
        strcpy(s.surname, surname.c_str());

        s.age = ageDist(gen);
        s.grade = gradeDist(gen);

        // Write structure to file
        outputFile.write(reinterpret_cast<char*>(&s), sizeof(Student));

        // Display what was written
        std::cout << "Written record " << i + 1 << ": "
            << s.surname << " " << s.rose
            << ", " << s.age << " years, grade: "
            << std::fixed << std::setprecision(2) << s.grade << std::endl;
    }

    outputFile.close();

    // 2. READING FROM FILE
    std::cout << "\n=== READING DATA ===" << std::endl;

    std::ifstream inputFile("students.bin", std::ios::binary);

    if (!inputFile) {
        std::cerr << "Error opening file for reading!" << std::endl;
        return 1;
    }

    // Display table header
    std::cout << std::left << std::setw(15) << "Surname"
        << std::setw(15) << "Name"
        << std::setw(8) << "Age"
        << std::setw(10) << "Grade" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    Student s;
    int counter = 0;

    while (inputFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        std::cout << std::left << std::setw(15) << s.surname
            << std::setw(15) << s.rose
            << std::setw(8) << s.age
            << std::right << std::setw(9) << std::fixed << std::setprecision(2)
            << s.grade << std::endl;
        counter++;
    }

    inputFile.close();

    // 3. DISPLAY FILE SIZE
    std::cout << "\n=== FILE INFORMATION ===" << std::endl;

    std::ifstream file("students.bin", std::ios::binary | std::ios::ate);

    if (file) {
        std::streampos size = file.tellg();
        std::cout << "Number of records read: " << counter << std::endl;
        std::cout << "Size of one structure: " << sizeof(Student) << " bytes" << std::endl;
        std::cout << "Total file size: " << size << " bytes" << std::endl;
        std::cout << "Verification: " << counter << " × " << sizeof(Student)
            << " = " << (counter * sizeof(Student)) << " bytes" << std::endl;
        file.close();
    }

    return 0;
}