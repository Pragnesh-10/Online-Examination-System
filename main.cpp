#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <chrono>

using namespace std;

// -------------------- EXAM SETTINGS --------------------
// Change these constants to customize the examination.
const int EXAM_DURATION_MINUTES = 10;
const double MARKS_FOR_CORRECT_ANSWER = 1.0;
const double PENALTY_FOR_WRONG_ANSWER = 0.25;
const double PASS_PERCENTAGE = 40.0;

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
    double score;
    int totalMarks;

public:
    Student(string n) : User(n) {
        score = 0.0;
        totalMarks = 0;
    }

    void displayRole() override {
        cout << "\nStudent Name: " << name << endl;
    }

    string getName() {
        return name;
    }

    void setResult(double s, int total) {
        score = s;
        totalMarks = total;
    }

    void displayResult() {
        double percentage = 0.0;

        if (totalMarks > 0) {
            percentage = (score / totalMarks) * 100.0;
        }

        cout << "\n========== EXAM RESULT ==========\n";
        cout << "Student Name : " << name << endl;
        cout << fixed << setprecision(2);
        cout << "Score        : " << score
             << "/" << totalMarks << endl;
        cout << "Percentage   : " << percentage << "%\n";
        cout << "Pass Mark    : " << PASS_PERCENTAGE << "%\n";
        cout << "Status       : "
             << (percentage >= PASS_PERCENTAGE ? "PASS" : "FAIL")
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
        if (!(cin >> ans)) {
            cout << "Invalid input. Question not added.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

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

        double score = 0.0;
        int correctCount = 0;
        int wrongCount = 0;
        int unansweredCount = 0;

        const auto startTime = chrono::steady_clock::now();
        const auto deadline = startTime +
            chrono::minutes(EXAM_DURATION_MINUTES);

        cout << "\n========== EXAM STARTED ==========\n";
        cout << "Student: " << student.getName() << endl;
        cout << "Total Questions: " << questions.size() << endl;
        cout << "Time Limit: " << EXAM_DURATION_MINUTES << " minutes\n";
        cout << "Correct Answer: +" << MARKS_FOR_CORRECT_ANSWER << " marks\n";
        cout << "Wrong Answer: -" << PENALTY_FOR_WRONG_ANSWER << " marks\n";
        cout << "Unanswered: 0 marks\n";
        cout << "The exam will be submitted automatically when time expires.\n";

        for (int i = 0; i < static_cast<int>(questions.size()); i++) {
            auto now = chrono::steady_clock::now();

            if (now >= deadline) {
                cout << "\nTime is up! Submitting your exam automatically.\n";
                unansweredCount += static_cast<int>(questions.size()) - i;
                break;
            }

            auto remainingSeconds =
                chrono::duration_cast<chrono::seconds>(deadline - now).count();

            cout << "\nTime remaining: "
                 << remainingSeconds / 60 << " minute(s) "
                 << remainingSeconds % 60 << " second(s)\n";

            questions[i].displayQuestion(i + 1);

            int answer = 0;
            bool validAnswer = false;

            while (!validAnswer) {
                now = chrono::steady_clock::now();

                if (now >= deadline) {
                    cout << "\nTime is up! Submitting your exam automatically.\n";
                    unansweredCount +=
                        static_cast<int>(questions.size()) - i;
                    break;
                }

                cout << "Enter your answer (1-4): ";

                if (cin >> answer && answer >= 1 && answer <= 4) {
                    validAnswer = true;
                } else {
                    cout << "Invalid input. Enter a number from 1 to 4.\n";
                    cin.clear();
                    cin.ignore(
                        numeric_limits<streamsize>::max(), '\n'
                    );
                }
            }

            if (!validAnswer) {
                break;
            }

            if (questions[i].checkAnswer(answer)) {
                score += MARKS_FOR_CORRECT_ANSWER;
                correctCount++;
                cout << "Correct answer! +" << MARKS_FOR_CORRECT_ANSWER
                     << " marks.\n";
            } else {
                score -= PENALTY_FOR_WRONG_ANSWER;
                wrongCount++;
                cout << "Incorrect answer. -" << PENALTY_FOR_WRONG_ANSWER
                     << " marks.\n";
            }

            // Do not allow the final score to become negative.
            if (score < 0.0) {
                score = 0.0;
            }
        }

        // Any questions not attempted because time expired are unanswered.
        int attemptedCount = correctCount + wrongCount;
        unansweredCount = static_cast<int>(questions.size()) - attemptedCount;

        // The maximum possible score is one mark per question.
        student.setResult(
            score, static_cast<int>(questions.size())
        );

        cout << "\nExam submitted successfully!\n";
        cout << "\nAnswer Summary:\n";
        cout << "Correct answers   : " << correctCount << endl;
        cout << "Wrong answers     : " << wrongCount << endl;
        cout << "Unanswered        : " << unansweredCount << endl;
        cout << "Negative marking  : -" << PENALTY_FOR_WRONG_ANSWER
             << " per wrong answer\n";
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
