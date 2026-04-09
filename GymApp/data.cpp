#include <iostream>
#include <windows.h>
#include <iomanip>
#include "data.h"

using namespace std;

// --- CÁC HÀM TIỆN ÍCH DÙNG WINDOWS.H ---

// Hàm thay đổi màu sắc văn bản trong Console
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Bảng màu cơ bản:
// 10: Xanh lá sáng | 11: Xanh lơ sáng | 12: Đỏ sáng | 14: Vàng | 15: Trắng

// Hàm xóa màn hình
void clearScreen() {
    system("cls");
}

// Hàm dừng màn hình
void pauseScreen() {
    setColor(15); // Trắng
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore();
    cin.get();
}

// Hàm in tiêu đề
void printHeader(string title) {
    clearScreen();
    setColor(11); // Xanh lơ
    cout << "====================================================\n";
    cout << "   " << title << "\n";
    cout << "====================================================\n\n";
    setColor(15);
}

// --- HÀM MAIN ---
int main() {
    // Thiết lập tiếng Việt cho console (nếu hỗ trợ)
    SetConsoleOutputCP(CP_UTF8);

    DataManager db;
    User currentUser;
    string username;

    // 1. MÀN HÌNH ĐĂNG NHẬP / ĐĂNG KÝ
    printHeader("HE THONG QUAN LY TAP LUYEN & DINH DUONG");
    setColor(14); // Vàng
    cout << "Nhap ten dang nhap cua ban: ";
    setColor(15);
    cin >> username;

    if (db.loadUser(username, currentUser)) {
        setColor(10); // Xanh lá
        cout << ">> Dang nhap thanh cong! Chao mung tro lai, " << currentUser.username << "!\n";
    } else {
        setColor(12); // Đỏ
        cout << ">> Tai khoan chua ton tai. Dang ky tai khoan moi.\n";
        setColor(15);
        currentUser.username = username;
        cout << "Nhap chieu cao (cm): ";
        cin >> currentUser.height;
        cout << "Nhap can nang (kg): ";
        cin >> currentUser.weight;

        if (db.saveUser(currentUser)) {
            setColor(10);
            cout << ">> Dang ky thanh cong!\n";
        } else {
            setColor(12);
            cout << ">> Loi: Khong the luu thong tin nguoi dung!\n";
            return 1;
        }
    }
    cin.ignore(); // Xóa bộ đệm
    pauseScreen();

    // 2. VÒNG LẶP MENU CHÍNH
    int choice = 0;
    do {
        printHeader("MENU CHINH - Nguoi dung: " + currentUser.username);
        setColor(14);
        cout << "[1]. Xem ho so & Phac do dinh duong\n";
        cout << "[2]. Ghi lai buoi tap moi\n";
        cout << "[3]. Xem lich su tap luyen\n";
        cout << "[4]. Goi y bai tap thay the tai nha\n";
        cout << "[5]. Xuat du lieu lich su ra file Excel (CSV)\n";
        cout << "[0]. Thoat\n";
        setColor(15);
        cout << "\n>> Nhap lua chon cua ban: ";
        cin >> choice;
        cin.ignore(); // Xóa phím Enter trong bộ đệm

        switch (choice) {
            case 1: {
                printHeader("HO SO & PHAC DO DINH DUONG");
                setColor(10);
                cout << "- Chieu cao: " << currentUser.height << " cm\n";
                cout << "- Can nang: " << currentUser.weight << " kg\n";

                setColor(11);
                cout << "\n[ CHISO BMI ]\n";
                setColor(15);
                cout << "BMI cua ban: " << fixed << setprecision(2) << currentUser.getBMI() << "\n";
                cout << "Phan loai: " << currentUser.getBMICategory() << "\n";

                setColor(14);
                cout << "\n[ PHAC DO DINH DUONG & TAP LUYEN ]\n";
                setColor(15);
                cout << currentUser.getNutritionAdvice() << "\n";
                pauseScreen();
                break;
            }
            case 2: {
                printHeader("GHI LAI BUOI TAP");
                Workout w;
                w.username = currentUser.username;

                cout << "Loai hinh tap (VD: Gym, Calisthenics, Cardio): ";
                getline(cin, w.type);
                cout << "Ten bai tap (VD: Bench Press): ";
                getline(cin, w.name);
                cout << "Nhom co (VD: Nguc, Lung, Xo): ";
                getline(cin, w.category);
                cout << "Do kho (De, Trung Binh, Kho): ";
                getline(cin, w.difficulty);

                cout << "So hiep (Sets): ";
                cin >> w.sets;
                cout << "So lan/hiep (Reps): ";
                cin >> w.reps;
                cout << "Khoi luong ta (kg) [Nhap 0 neu tap bodyweight]: ";
                cin >> w.weight;

                w.calculate(); // Tính tổng Volume
                db.saveWorkout(w);

                setColor(10);
                cout << "\n>> Luu bai tap thanh cong! Tong Volume: " << w.volume << " kg\n";
                pauseScreen();
                break;
            }
            case 3: {
                printHeader("LICH SU TAP LUYEN");
                vector<Workout> history = db.loadHistory(currentUser.username);

                if (history.empty()) {
                    setColor(12);
                    cout << "Chua co du lieu tap luyen nao!\n";
                } else {
                    // In bảng hiển thị
                    setColor(11);
                    cout << left << setw(20) << "Ten Bai Tap"
                         << setw(15) << "Nhom Co"
                         << setw(10) << "Sets"
                         << setw(10) << "Reps"
                         << setw(15) << "Khoi Luong(kg)"
                         << setw(15) << "Volume" << "\n";
                    cout << "--------------------------------------------------------------------------------\n";
                    setColor(15);
                    for (const auto& w : history) {
                        cout << left << setw(20) << w.name
                             << setw(15) << w.category
                             << setw(10) << w.sets
                             << setw(10) << w.reps
                             << setw(15) << w.weight
                             << setw(15) << w.volume << "\n";
                    }
                }
                pauseScreen();
                break;
            }
            case 4: {
                printHeader("GOI Y BAI TAP TAI NHA (HOME ALTERNATIVES)");
                setColor(14);
                cout << "Cac bai tap thay the khi ban khong the den phong Gym:\n\n";
                setColor(15);

                for (const auto& pair : db.homeAlternatives) {
                    setColor(11); cout << pair.first; // Tên bài Gym
                    setColor(15); cout << " --> ";
                    setColor(10); cout << pair.second << "\n"; // Bài thay thế tại nhà
                }
                cout << "\n";
                pauseScreen();
                break;
            }
            case 5: {
                printHeader("XUAT DU LIEU RA EXCEL (CSV)");
                if (db.exportToCSV(currentUser.username)) {
                    setColor(10);
                    cout << ">> Xuat du lieu thanh cong!\n";
                    cout << ">> Ten file: " << currentUser.username << "_LichSu.csv\n";
                    cout << ">> (Ban co the mo file nay bang Excel hoac Google Sheets)\n";
                } else {
                    setColor(12);
                    cout << ">> Loi: Khong co du lieu hoac khong the tao file!\n";
                }
                pauseScreen();
                break;
            }
            case 0:
                setColor(10);
                cout << "\nCam on ban da su dung phan mem. Chuc ban tap luyen hieu qua!\n";
                break;
            default:
                setColor(12);
                cout << "Lua chon khong hop le. Vui long thu lai!\n";
                pauseScreen();
        }
    } while (choice != 0);

    return 0;
}
