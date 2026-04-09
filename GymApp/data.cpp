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

    double getBMI() const {
        if (height <= 0) return 0; // Tranh loi chia cho 0
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

    // TÍNH NĂNG MỚI: Phác đồ dinh dưỡng
    string getNutritionAdvice() const {
        double bmi = getBMI();
        if (bmi < 18.5) return ">> Trang thai: Thieu can\n>> Loi khuyen: Tang Calo (+500 kcal/ngay). An nhieu tinh bot, protein.\n>> Tap luyen: Tap trung cac bai Compound nang (Squat, Bench Press), it hiep, it lan.";
        if (bmi >= 18.5 && bmi <= 24.9) return ">> Trang thai: Ly tuong\n>> Loi khuyen: Duy tri Calo hien tai. An uong can bang du chat.\n>> Tap luyen: Ket hop linh hoat giua nang ta (Hypertrophy) va tim mach (Cardio).";
        if (bmi >= 25 && bmi <= 29.9) return ">> Trang thai: Thua can\n>> Loi khuyen: Giam nhe Calo (-300 kcal/ngay). Han che tinh bot xau, do ngot.\n>> Tap luyen: Tang cuong do tap, giam thoi gian nghi, them 15-20 phut Cardio cuoi buoi.";
        return ">> Trang thai: Beo phi\n>> Loi khuyen: Tham hut Calo (-500 kcal/ngay). Ap dung an kieng nghiem ngat.\n>> Tap luyen: Tap trung Cardio dot mo (Chay bo, Dap xe), han che cac bai de gay ap luc len khop.";
    }
};

// --- PHAN LOAI BAI TAP ---
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

// --- Database Manager ---
class DataManager {
public:
    const string resourceFile = "resources.txt";
    const string userFile = "users.txt";

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

    bool loadUser(const string& username, User& u) {
        ifstream file(userFile);
        if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue; 
            stringstream ss(line);
            string uName, h, w;
            getline(ss, uName, '|');
            if (uName == username) {
                try { 
                    getline(ss, h, '|'); getline(ss, w, '|');
                    u.username = uName; u.height = stod(h); u.weight = stod(w);
                    return true;
                } catch (...) { return false; }
            }
        }
        return false;
    }

    bool saveUser(const User& u) {
        ofstream file(userFile, ios::app);
        if (!file.is_open()) return false; 
        file << u.username << "|" << u.height << "|" << u.weight << endl;
        file.close(); 
        return true;
    }

    void saveWorkout(const Workout& w) {
        ofstream file(resourceFile, ios::app);
        if(file.is_open()){
            file << w.toCsv() << endl;
            file.close();
        }
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
                    Workout w;
                    w.username = p[0]; w.type = p[1]; w.name = p[2]; w.category = p[3]; 
                    w.difficulty = p[4]; w.sets = stoi(p[5]); w.reps = stoi(p[6]); 
                    w.weight = stod(p[7]); w.volume = stod(p[8]);
                    history.push_back(w);
                } catch (...) { continue; }
            }
        }
        return history;
    }

    // TÍNH NĂNG MỚI: Xuất dữ liệu ra Excel (File .csv)
    bool exportToCSV(const string& username) {
        vector<Workout> history = loadHistory(username);
        if (history.empty()) return false; // Không có lịch sử để xuất

        string filename = username + "_LichSu.csv";
        ofstream file(filename); // Chế độ ghi đè tạo file mới
        if (!file.is_open()) return false;

        // Ghi dòng tiêu đề cột cho Excel hiểu
        file << "Username,Loai,Ten Bai Tap,Nhom Co,Do Kho,So Hiep,So Lan,Khoi Luong(kg),Tong Volume" << endl;
        
        // Ghi dữ liệu, thay dấu | bằng dấu ,
        for (const auto& w : history) {
            file << w.username << "," << w.type << "," << w.name << "," 
                 << w.category << "," << w.difficulty << "," << w.sets << "," 
                 << w.reps << "," << w.weight << "," << w.volume << endl;
        }
        file.close();
        return true;
    }
};

#endif