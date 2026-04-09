#ifndef DATA_CPP
#define DATA_CPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <windows.h>

using namespace std;

// ---NGUOI DUNG ---
class User {
public:
    string username;
    double height; // cm
    double weight; // kg
    int age = 20;  // Mac dinh
    string gender = "Nam"; 

    double getBMI() const {
        if (height <= 0) return 0;
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

    double getBMR() const {
        if (gender == "Nam")
            return (10 * weight) + (6.25 * height) - (5 * age) + 5;
        else
            return (10 * weight) + (6.25 * height) - (5 * age) - 161;
    }

    double getTDEE(double activityMultiplier) const {
        return getBMR() * activityMultiplier;
    }

    string getNutritionAdvice() const {
        double bmi = getBMI();
        if (bmi < 18.5) return ">> Trang thai: Thieu can\n>> Loi khuyen: Tang Calo (+500 kcal/ngay). An nhieu tinh bot, protein.\n>> Tap luyen: Tap trung cac bai Compound nang (Squat, Bench Press).";
        if (bmi >= 18.5 && bmi <= 24.9) return ">> Trang thai: Ly tuong\n>> Loi khuyen: Duy tri Calo hien tai. An uong can bang du chat.\n>> Tap luyen: Ket hop nang ta (Hypertrophy) va tim mach (Cardio).";
        if (bmi >= 25 && bmi <= 29.9) return ">> Trang thai: Thua can\n>> Loi khuyen: Giam nhe Calo (-300 kcal/ngay). Han che tinh bot xau, do ngot.\n>> Tap luyen: Tang cuong do tap, them 15-20 phut Cardio cuoi buoi.";
        return ">> Trang thai: Beo phi\n>> Loi khuyen: Tham hut Calo (-500 kcal/ngay). Ap dung an kieng nghiem ngat.\n>> Tap luyen: Tap trung Cardio dot mo (Chay bo, Dap xe).";
    }
};

class Workout {
public:
    string username, type, name, category, difficulty;
    int sets, reps;
    double weight, volume;

    Workout() : sets(0), reps(0), weight(0.0), volume(0.0) {}
    void calculate() { volume = (double)sets * reps * weight; }

    string toCsv() const {
        return username + "|" + type + "|" + name + "|" + category + "|" + difficulty + "|" +
               to_string(sets) + "|" + to_string(reps) + "|" + to_string(weight) + "|" + to_string(volume);
    }
};

class DataManager {
public:
    const string resourceFile = "resources.txt";
    const string userFile = "users.txt";

    map<string, string> homeAlternatives = {
        {"Bench Press", "Push-ups / Archer Push-ups (Compound - Nguc, Vai)"},
        {"Barbell Squat", "Bodyweight Squat / Pistol Squat (Compound - Dui, Mong)"},
        {"Deadlift", "Glute Bridges / Superman (Compound - Lung duoi)"},
        {"Lat Pulldown", "Pull-ups / Chin-ups (Compound - Lung xo)"},
        {"Overhead Press", "Pike Push-ups / Handstand (Compound - Vai)"},
        {"Bicep Curls", "Resistance Band Curls / Towel Curls (Isolate - Tay truoc)"},
        {"Tricep Extension", "Bench Dips / Diamond Push-ups (Isolate - Tay sau)"},
        {"Leg Extension", "Sissy Squats (Isolate - Dui truoc)"}
    };

    // DA FIX BUG: Quet toan bo file de lay du lieu moi nhat (Dong cuoi cung)
    bool loadUser(const string& username, User& u) {
        ifstream file(userFile);
        if (!file.is_open()) return false;
        
        string line;
        bool found = false;
        
        while (getline(file, line)) {
            if (line.empty()) continue; 
            stringstream ss(line);
            string uName, h, w, ageStr, genStr;
            getline(ss, uName, '|');
            
            if (uName == username) {
                try { 
                    getline(ss, h, '|'); getline(ss, w, '|');
                    u.username = uName; u.height = stod(h); u.weight = stod(w);
                    if (getline(ss, ageStr, '|') && getline(ss, genStr, '|')) {
                        u.age = stoi(ageStr);
                        u.gender = genStr;
                    } else {
                        u.age = 20; u.gender = "Nam"; 
                    }
                    found = true; // Danh dau la da thay, tiep tuc chay de cap nhat dong moi
                } catch (...) { continue; }
            }
        }
        return found;
    }

    bool saveUser(const User& u) {
        ofstream file(userFile, ios::app); // Ghi noi tiep xuong dong cuoi
        if (!file.is_open()) return false; 
        file << u.username << "|" << u.height << "|" << u.weight << "|" << u.age << "|" << u.gender << endl;
        file.close(); 
        return true;
    }

    void saveWorkout(const Workout& w) {
        ofstream file(resourceFile, ios::app);
        if(file.is_open()){ file << w.toCsv() << endl; file.close(); }
    }

    vector<Workout> loadHistory(const string& current_user) {
        vector<Workout> history;
        ifstream file(resourceFile);
        if (!file.is_open()) return history;
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line); string item; vector<string> p;
            while (getline(ss, item, '|')) p.push_back(item);
            if (p.size() >= 9 && p[0] == current_user) {
                try {
                    Workout w; w.username = p[0]; w.type = p[1]; w.name = p[2]; w.category = p[3]; 
                    w.difficulty = p[4]; w.sets = stoi(p[5]); w.reps = stoi(p[6]); 
                    w.weight = stod(p[7]); w.volume = stod(p[8]);
                    history.push_back(w);
                } catch (...) { continue; }
            }
        }
        return history;
    }

    bool exportToCSV(const string& username) {
        vector<Workout> history = loadHistory(username);
        if (history.empty()) return false;
        string filename = username + "_LichSu.csv";
        ofstream file(filename);
        if (!file.is_open()) return false;
        file << "Username,Loai,Ten Bai Tap,Nhom Co,Do Kho,So Hiep,So Lan,Khoi Luong(kg),Tong Volume" << endl;
        for (const auto& w : history) {
            file << w.username << "," << w.type << "," << w.name << "," << w.category << "," << w.difficulty << "," << w.sets << "," << w.reps << "," << w.weight << "," << w.volume << endl;
        }
        file.close();
        return true;
    }

    string getTrainingAdvice(const string& goal) {
        string result = "=== GIAO AN CHUAN KHOA HOC ===\nMuc tieu: " + goal + "\n\n";
        if (goal == "Hypertrophy (Tang co)") {
            result += "- So hiep (Sets): 3 den 4 hiep/bai.\n- So lan (Reps): 8 den 12 lan/hiep.\n- Trong luong ta: 65% - 80% 1RM.\n- Thoi gian nghi: 60 - 90 giay.\n\n=> Ban chat: Kich thich phat trien the tich co bap (Time under tension).";
        } else if (goal == "Strength (Tang suc manh)") {
            result += "- So hiep (Sets): 3 den 5 hiep/bai.\n- So lan (Reps): 1 den 5 lan/hiep.\n- Trong luong ta: 85% - 95% 1RM.\n- Thoi gian nghi: 3 - 5 phut.\n\n=> Ban chat: Huan luyen he than kinh trung uong (CNS) phat luc toi da.";
        } else if (goal == "Endurance (Tang suc ben)") {
            result += "- So hiep (Sets): 2 den 3 hiep/bai.\n- So lan (Reps): 15 den 20+ lan/hiep.\n- Trong luong ta: Duoi 65% 1RM (Ta nhe).\n- Thoi gian nghi: 30 - 45 giay.\n\n=> Ban chat: Tang suc chiu dung, dao thai axit lactic.";
        }
        return result;
    }
};
#endif