
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

// Abstract Base Class
class User {
protected:
    string name;

public:
    User(string n) {
        name = n;
    }

    virtual void displayRole() = 0;

    virtual ~User() {}
};

// Student Class
class Student : public User {
private:
    int score;
    int totalMarks;

public:
    Student(string n) : User(n) {
        score = 0;
        totalMarks = 0;
    }

    void displayRole() override {
        cout << "\nStudent Name: " << name << endl;
    }

    string getName() {
        return name;
    }

    void setResult(int s, int total) {
        score = s;
        totalMarks = total;
    }

    void displayResult() {
        double percentage = 0;

        if (totalMarks > 0) {
            percentage =
                (static_cast<double>(score) / totalMarks) * 100;
        }

        cout << "\n========== EXAM RESULT ==========\n";
        cout << "Student Name : " << name << endl;
        cout << "Score        : " << score
             << "/" << totalMarks << endl;
        cout << fixed << setprecision(2);
        cout << "Percentage   : " << percentage << "%\n";
        cout << "Status       : "
             << (percentage >= 40 ? "PASS" : "FAIL")
             << endl;
        cout << "=================================\n";
    }
};

// Question Class
class Question {
private:
    string questionText;
    string options[4];
    int correctAnswer;

public:
    Question(string q, string a, string b,
             string c, string d, int ans) {
        questionText = q;
        options[0] = a;
        options[1] = b;
        options[2] = c;
        options[3] = d;
        correctAnswer = ans;
    }

    void displayQuestion(int number) {
        cout << "\nQ" << number << ". "
             << questionText << endl;

        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ". "
                 << options[i] << endl;
        }
    }

    bool checkAnswer(int answer) {
        return answer == correctAnswer;
    }
};

// Admin Class
class Admin : public User {
public:
    Admin(string n) : User(n) {}

    void displayRole() override {
        cout << "\nWelcome, Administrator "
             << name << "!\n";
    }

    bool login() {
        string username, password;

        cout << "\nEnter admin username: ";
        cin >> username;

        cout << "Enter admin password: ";
        cin >> password;

        if (username == "admin" &&
            password == "admin123") {
            displayRole();
            return true;
        }

        cout << "\nInvalid username or password!\n";
        return false;
    }
};

// Exam Class
class Exam {
private:
    vector<Question> questions;

public:
    void addQuestion() {
        string q, a, b, c, d;
        int ans;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nEnter question: ";
        getline(cin, q);

        cout << "Enter option 1: ";
        getline(cin, a);

        cout << "Enter option 2: ";
        getline(cin, b);

        cout << "Enter option 3: ";
        getline(cin, c);

        cout << "Enter option 4: ";
        getline(cin, d);

        cout << "Enter correct option (1-4): ";
        cin >> ans;

        if (ans < 1 || ans > 4) {
            cout << "Invalid answer. Question not added.\n";
            return;
        }

        questions.push_back(
            Question(q, a, b, c, d, ans)
        );

        cout << "\nQuestion added successfully!\n";
    }

    void conductExam(Student &student) {
        if (questions.empty()) {
            cout << "\nNo questions available.\n";
            return;
        }

        int score = 0;

        cout << "\n========== EXAM STARTED ==========\n";
        cout << "Student: " << student.getName() << endl;
        cout << "Total Questions: "
             << questions.size() << endl;

        for (int i = 0; i < static_cast<int>(questions.size()); i++) {
            questions[i].displayQuestion(i + 1);

            int answer;

            while (true) {
                cout << "Enter your answer (1-4): ";

                if (cin >> answer &&
                    answer >= 1 && answer <= 4) {
                    break;
                }

                cout << "Invalid input. Enter a number from 1 to 4.\n";
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );
            }

            if (questions[i].checkAnswer(answer)) {
                score++;
            }
        }

        student.setResult(
            score, static_cast<int>(questions.size())
        );

        cout << "\nExam submitted successfully!\n";
        student.displayResult();
    }

    void showQuestionCount() {
        cout << "\nTotal questions available: "
             << questions.size() << endl;
    }
};

// Main Function
int main() {
    Exam exam;
    Admin admin("Admin");

    int choice;

    do {
        cout << "\n====================================\n";
        cout << "   ONLINE EXAMINATION SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Admin Login\n";
        cout << "2. Student Examination\n";
        cout << "3. Show Total Questions\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Enter a number.\n";
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            continue;
        }

        switch (choice) {
            case 1: {
                if (admin.login()) {
                    int n;

                    cout << "\nHow many questions do you want to add? ";
                    if (!(cin >> n)) {
                        cout << "Invalid input.\n";
                        cin.clear();
                        cin.ignore(
                            numeric_limits<streamsize>::max(), '\n'
                        );
                        break;
                    }

                    if (n < 0) {
                        cout << "Number of questions cannot be negative.\n";
                        break;
                    }

                    for (int i = 0; i < n; i++) {
                        cout << "\nAdding Question "
                             << i + 1 << " of " << n << endl;
                        exam.addQuestion();
                    }
                }
                break;
            }

            case 2: {
                string studentName;

                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );

                cout << "\nEnter student name: ";
                getline(cin, studentName);

                if (studentName.empty()) {
                    cout << "Student name cannot be empty.\n";
                    break;
                }

                Student student(studentName);
                student.displayRole();
                exam.conductExam(student);
                break;
            }

            case 3:
                exam.showQuestionCount();
                break;

            case 4:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
