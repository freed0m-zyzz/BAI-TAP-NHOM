#ifndef DATA_CPP
#define DATA_CPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>

using namespace std;

// ---NGUOI DUNG ---
class User {
public:
    string username;
    double height; // cm
    double weight; // kg

    double getBMI() const {
        double h_meters = height / 100.0;
        return weight / (h_meters * h_meters);
    }

    string getBMICategory() const {
        double bmi = getBMI();
        if (bmi < 18.5) return "Gay (Underweight)";
        if (bmi >= 18.5 && bmi <= 24.9) return "Binh thuong (Normal)";
        if (bmi >= 25 && bmi <= 29.9) return "Thua can (Overweight)";
        return "Beo phi (Obese)";
    }
};

// --- PHAN LOAI BAI TAP ---
class Workout {
public:
    string username, type, name, category, difficulty;
    int sets, reps;
    double weight, volume;

    Workout() : sets(0), reps(0), weight(0.0), volume(0.0) {}
    void calculate() { volume = sets * reps * weight; }

    string toCsv() const {
        return username + "|" + type + "|" + name + "|" + category + "|" + difficulty + "|" +
               to_string(sets) + "|" + to_string(reps) + "|" + to_string(weight) + "|" + to_string(volume);
    }
};

// --- Database Manager ---
class DataManager {
public:
    const string resourceFile = "resources.txt";
    const string userFile = "users.txt";

    // --- TỪ ĐIỂN BÀI TẬP THAY THẾ (GYM -> HOME) ---
    map<string, string> homeAlternatives = {
        {"Bench Press", "Push-ups / Archer Push-ups (Compound - Nguc, Vai, Tay sau)"},
        {"Barbell Squat", "Bodyweight Squat / Pistol Squat (Compound - Dui, Mong)"},
        {"Deadlift", "Glute Bridges / Superman (Compound - Lung duoi, Mong)"},
        {"Lat Pulldown", "Pull-ups / Chin-ups (Compound - Lung xo, Tay truoc)"},
        {"Overhead Press", "Pike Push-ups / Handstand (Compound - Vai)"},
        {"Bicep Curls", "Resistance Band Curls / Towel Curls (Isolate - Tay truoc)"},
        {"Tricep Extension", "Bench Dips / Diamond Push-ups (Isolate - Tay sau)"},
        {"Leg Extension", "Sissy Squats (Isolate - Dui truoc)"}
    };

    // --- Quản lý User (users.txt: username|height|weight) ---
    bool loadUser(const string& username, User& u) {
        ifstream file(userFile);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string uName, h, w;
            getline(ss, uName, '|');
            if (uName == username) {
                getline(ss, h, '|'); getline(ss, w, '|');
                u.username = uName; u.height = stod(h); u.weight = stod(w);
                return true;
            }
        }
        return false;
    }

    void saveUser(const User& u) {
        ofstream file(userFile, ios::app);
        file << u.username << "|" << u.height << "|" << u.weight << endl;
    }

    // --- Quản lý Nhật ký (resources.txt) ---
    void saveWorkout(const Workout& w) {
        ofstream file(resourceFile, ios::app);
        file << w.toCsv() << endl;
    }

    vector<Workout> loadHistory(const string& current_user) {
        vector<Workout> history;
        ifstream file(resourceFile);
        string line;
        while (getline(file, line)) {
            stringstream ss(line); string item; vector<string> p;
            while (getline(ss, item, '|')) p.push_back(item);
            if (p.size() == 9 && p[0] == current_user) {
                Workout w;
                w.username = p[0]; w.type = p[1]; w.name = p[2]; w.category = p[3]; 
                w.difficulty = p[4]; w.sets = stoi(p[5]); w.reps = stoi(p[6]); 
                w.weight = stod(p[7]); w.volume = stod(p[8]);
                history.push_back(w);
            }
        }
        return history;
    }
};

#endif