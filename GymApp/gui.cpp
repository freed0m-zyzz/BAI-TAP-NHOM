#include <iomanip>
#include "data.cpp"

// ANSI Colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

class GymUI {
private:
    DataManager db;
    User currentUser;

    void header(string title) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        cout << CYAN << BOLD << "=== " << title << " ===" << RESET << endl << endl;
    }

    void pressEnter() {
        cout << "\nNhan Enter de tiep tuc...";
        cin.ignore(1000, '\n'); cin.get();
    }

public:
    void start() {
        header("POWERLOG - DANG NHAP");
        string username;
        cout << "Nhap ten dang nhap cua ban (Viet lien khong dau): ";
        cin >> username;

        if (!db.loadUser(username, currentUser)) {
            cout << YELLOW << "\nTai khoan moi! Vui long nhap thong tin the trang:" << RESET << endl;
            currentUser.username = username;
            cout << "Chieu cao (cm): "; cin >> currentUser.height;
            cout << "Can nang (kg): "; cin >> currentUser.weight;
            db.saveUser(currentUser);
            cout << GREEN << "Tao ho so thanh cong!" << RESET << endl;
        } else {
            cout << GREEN << "\nChao mung tro lai, " << currentUser.username << "!" << RESET << endl;
        }
        pressEnter();
        mainMenu();
    }

    void showProfileBar() {
        double bmi = currentUser.getBMI();
        cout << MAGENTA << "┌────────────────────────────────────────────────────────┐" << RESET << endl;
        cout << MAGENTA << "│" << RESET << BOLD << " PROFILE: " << currentUser.username 
             << " | Height: " << currentUser.height << "cm | Weight: " << currentUser.weight << "kg" 
             << MAGENTA << setw(8) << " │" << RESET << endl;
        cout << MAGENTA << "│" << RESET << BOLD << " BMI: " << fixed << setprecision(1) << bmi 
             << " -> " << YELLOW << currentUser.getBMICategory() << MAGENTA << setw(18) << " │" << RESET << endl;
        cout << MAGENTA << "└────────────────────────────────────────────────────────┘" << RESET << endl << endl;
    }

    void mainMenu() {
        int choice;
        do {
            header("POWERLOG - MAIN MENU");
            showProfileBar(); // Hiển thị khung thông tin sức khỏe

            cout << "1. " << GREEN << "Ghi chep buoi tap" << RESET << endl;
            cout << "2. " << YELLOW << "Xem nhat ky & Tong Volume" << RESET << endl;
            cout << "3. " << CYAN << "Tra cuu bai tap thay the (Gym -> Home)" << RESET << endl;
            cout << "4. " << RED << "Dang xuat & Thoat" << RESET << endl;
            cout << "\nChon option: "; cin >> choice;

            if (choice == 1) addEntry();
            else if (choice == 2) showLogs();
            else if (choice == 3) suggestAlternatives();
            
        } while (choice != 4);
    }

    void suggestAlternatives() {
        header("GOC TU VAN: BAI TAP THAY THE TAI NHA");
        cout << "Neu ban khong the den Gym, day la cac bai tap tuong duong:\n\n";
        
        cout << BOLD << left << setw(20) << "BAI TAP GYM" << " | " << "BAI TAP TAI NHA (HOME ALTERNATIVE)" << RESET << endl;
        cout << "─────────────────────┼────────────────────────────────────────────────────────" << endl;
        
        for (const auto& pair : db.homeAlternatives) {
            cout << CYAN << left << setw(20) << pair.first << RESET 
                 << " | " << pair.second << endl;
        }
        pressEnter();
    }

    void addEntry() {
        header("THEM BAI TAP MOI");
        Workout w;
        w.username = currentUser.username;

        int t; cout << "Che do (1.Gym / 2.Home): "; cin >> t; w.type = (t == 1 ? "GYM" : "HOME");
        cin.ignore();
        cout << "Ten bai tap: "; getline(cin, w.name);
        
        int c; cout << "Loai (1.Compound / 2.Isolate): "; cin >> c; w.category = (c == 1 ? "Comp" : "Iso");
        int d; cout << "Do kho (1.Beginner / 2.Intermediate / 3.Advanced): "; cin >> d; 
        w.difficulty = (d == 1 ? "Beg" : (d == 2 ? "Int" : "Adv"));
        
        cout << "So hiep (Sets): "; cin >> w.sets;
        cout << "So lan (Reps): "; cin >> w.reps;
        cout << "Muc ta (kg - Nhap 0 neu dung Bodyweight): "; cin >> w.weight;

        // Nếu tập Bodyweight tại nhà, volume có thể tính dựa trên % cơ thể, nhưng ở đây ta cứ lấy số rep để đơn giản hóa.
        if (w.weight == 0) w.weight = currentUser.weight * 0.6; // Ước lượng bodyweight resistance

        w.calculate();
        db.saveWorkout(w);
        
        cout << GREEN << "\n[!] Luu thanh cong! Volume bai nay: " << BOLD << fixed << setprecision(1) << w.volume << "kg" << RESET << endl;
        pressEnter();
    }

    void showLogs() {
        header("NHAT KY TAP LUYEN - " + currentUser.username);
        vector<Workout> logs = db.loadHistory(currentUser.username);

        if (logs.empty()) {
            cout << RED << "Ban chua co du lieu tap luyen nao." << RESET << endl;
        } else {
            cout << "┌──────┬──────────────┬──────┬─────┬──────┬──────┬──────────┐" << endl;
            cout << "│ MODE │ NAME         │ TYPE │ LVL │ SETS │ REPS │ VOLUME   │" << endl;
            cout << "├──────┼──────────────┼──────┼─────┼──────┼──────┼──────────┤" << endl;
            
            double total = 0;
            for (const auto& w : logs) {
                cout << "│ " << left << setw(4) << w.type 
                     << " │ " << setw(12) << (w.name.length() > 12 ? w.name.substr(0,9)+"..." : w.name)
                     << " │ " << setw(4) << w.category 
                     << " │ " << setw(3) << w.difficulty
                     << " │ " << setw(4) << w.sets 
                     << " │ " << setw(4) << w.reps
                     << " │ " << YELLOW << setw(8) << fixed << setprecision(1) << w.volume << RESET << " │" << endl;
                total += w.volume;
            }
            cout << "└──────┴──────────────┴──────┴─────┴──────┴──────┴──────────┘" << endl;
            cout << ">>> TONG VOLUME TICH LUY: " << GREEN << BOLD << total << " kg" << RESET << endl;
        }
        pressEnter();
    }
};

int main() {
    GymUI app;
    app.start();
    return 0;
}