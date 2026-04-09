#include <windows.h>
#include <string>

// Goi file data.cpp
#include "data.cpp"

#define ID_TXT_USERNAME 101
#define ID_TXT_HEIGHT 102
#define ID_TXT_WEIGHT 103
#define ID_BTN_LOAD_USER 104
#define ID_BTN_SAVE_USER 105
#define ID_BTN_NUTRITION 106 // Nut moi

#define ID_TXT_EX_NAME 201
#define ID_TXT_SETS 202
#define ID_TXT_REPS 203
#define ID_TXT_EX_WEIGHT 204
#define ID_BTN_SAVE_WORKOUT 205
#define ID_BTN_VIEW_HISTORY 206
#define ID_BTN_SUGGEST_HOME 207
#define ID_BTN_EXPORT_CSV 208 // Nut moi

DataManager db;
HWND hUsername, hHeight, hWeight, hBmiResult;
HWND hExName, hSets, hReps, hExWeight;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // --- USER INFO ---
            CreateWindowA("STATIC", "THONG TIN NGUOI DUNG & BMI", WS_VISIBLE | WS_CHILD, 20, 20, 250, 20, hwnd, NULL, NULL, NULL);
            CreateWindowA("STATIC", "Ten User:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, NULL, NULL);
            hUsername = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 50, 150, 20, hwnd, (HMENU)ID_TXT_USERNAME, NULL, NULL);
            CreateWindowA("STATIC", "Chieu cao (cm):", WS_VISIBLE | WS_CHILD, 20, 80, 100, 20, hwnd, NULL, NULL, NULL);
            hHeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 80, 130, 20, hwnd, (HMENU)ID_TXT_HEIGHT, NULL, NULL);
            CreateWindowA("STATIC", "Can nang (kg):", WS_VISIBLE | WS_CHILD, 20, 110, 100, 20, hwnd, NULL, NULL, NULL);
            hWeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 110, 130, 20, hwnd, (HMENU)ID_TXT_WEIGHT, NULL, NULL);
            
            CreateWindowA("BUTTON", "Tai User", WS_VISIBLE | WS_CHILD, 20, 140, 90, 30, hwnd, (HMENU)ID_BTN_LOAD_USER, NULL, NULL);
            CreateWindowA("BUTTON", "Luu & Tinh BMI", WS_VISIBLE | WS_CHILD, 120, 140, 110, 30, hwnd, (HMENU)ID_BTN_SAVE_USER, NULL, NULL);
            CreateWindowA("BUTTON", "Tu Van Dinh Duong", WS_VISIBLE | WS_CHILD, 240, 140, 120, 30, hwnd, (HMENU)ID_BTN_NUTRITION, NULL, NULL);
            
            hBmiResult = CreateWindowA("STATIC", "BMI: Cho tinh toan...", WS_VISIBLE | WS_CHILD, 20, 180, 300, 20, hwnd, NULL, NULL, NULL);

            // --- WORKOUT LOG ---
            CreateWindowA("STATIC", "LUU NHAT KY BAI TAP", WS_VISIBLE | WS_CHILD, 380, 20, 200, 20, hwnd, NULL, NULL, NULL);
            CreateWindowA("STATIC", "Ten bai tap:", WS_VISIBLE | WS_CHILD, 380, 50, 80, 20, hwnd, NULL, NULL, NULL);
            hExName = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 470, 50, 150, 20, hwnd, (HMENU)ID_TXT_EX_NAME, NULL, NULL);
            CreateWindowA("STATIC", "So Hiep (Sets):", WS_VISIBLE | WS_CHILD, 380, 80, 100, 20, hwnd, NULL, NULL, NULL);
            hSets = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 490, 80, 130, 20, hwnd, (HMENU)ID_TXT_SETS, NULL, NULL);
            CreateWindowA("STATIC", "So Lan (Reps):", WS_VISIBLE | WS_CHILD, 380, 110, 100, 20, hwnd, NULL, NULL, NULL);
            hReps = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 490, 110, 130, 20, hwnd, (HMENU)ID_TXT_REPS, NULL, NULL);
            CreateWindowA("STATIC", "Ta (kg):", WS_VISIBLE | WS_CHILD, 380, 140, 100, 20, hwnd, NULL, NULL, NULL);
            hExWeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 490, 140, 130, 20, hwnd, (HMENU)ID_TXT_EX_WEIGHT, NULL, NULL);
            
            CreateWindowA("BUTTON", "Luu Bai Tap", WS_VISIBLE | WS_CHILD, 380, 170, 240, 30, hwnd, (HMENU)ID_BTN_SAVE_WORKOUT, NULL, NULL);
            
            CreateWindowA("BUTTON", "Xem Lich Su", WS_VISIBLE | WS_CHILD, 380, 210, 110, 30, hwnd, (HMENU)ID_BTN_VIEW_HISTORY, NULL, NULL);
            CreateWindowA("BUTTON", "Goi Y Tap Nha", WS_VISIBLE | WS_CHILD, 510, 210, 110, 30, hwnd, (HMENU)ID_BTN_SUGGEST_HOME, NULL, NULL);
            
            // Nut xuat CSV
            CreateWindowA("BUTTON", "Xuat du lieu ra Excel (.csv)", WS_VISIBLE | WS_CHILD, 380, 250, 240, 30, hwnd, (HMENU)ID_BTN_EXPORT_CSV, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                // ... (Phần tải/lưu User giữ nguyên)
                case ID_BTN_LOAD_USER: {
                    char nameBuf[100]; GetWindowTextA(hUsername, nameBuf, 100);
                    User u;
                    if (db.loadUser(nameBuf, u)) {
                        SetWindowTextA(hHeight, std::to_string(u.height).c_str());
                        SetWindowTextA(hWeight, std::to_string(u.weight).c_str());
                        std::string bmiRes = "BMI: " + std::to_string(u.getBMI()) + " (" + u.getBMICategory() + ")";
                        SetWindowTextA(hBmiResult, bmiRes.c_str());
                        MessageBoxA(hwnd, "Tai user thanh cong!", "Thong bao", MB_OK);
                    } else MessageBoxA(hwnd, "Khong tim thay nguoi dung nay!", "Loi", MB_ICONERROR);
                    break;
                }
                case ID_BTN_SAVE_USER: {
                    char nameBuf[100], hBuf[50], wBuf[50];
                    GetWindowTextA(hUsername, nameBuf, 100); GetWindowTextA(hHeight, hBuf, 50); GetWindowTextA(hWeight, wBuf, 50);
                    if (strlen(nameBuf) == 0 || strlen(hBuf) == 0 || strlen(wBuf) == 0) {
                        MessageBoxA(hwnd, "Vui long nhap day du thong tin!", "Thong bao", MB_ICONWARNING); break;
                    }
                    try {
                        User u; u.username = nameBuf; u.height = std::stod(hBuf); u.weight = std::stod(wBuf);
                        if (db.saveUser(u)) {
                            std::string bmiRes = "BMI: " + std::to_string(u.getBMI()) + " (" + u.getBMICategory() + ")";
                            SetWindowTextA(hBmiResult, bmiRes.c_str());
                            MessageBoxA(hwnd, "Luu thanh cong vao users.txt!", "Thanh cong", MB_OK);
                        } else MessageBoxA(hwnd, "Khong the mo file users.txt!", "Loi", MB_ICONERROR);
                    } catch (...) { MessageBoxA(hwnd, "Loi dinh dang so!", "Loi", MB_ICONERROR); }
                    break;
                }
                
                // TÍNH NĂNG MỚI 1: Nút Tư vấn Dinh Dưỡng
                case ID_BTN_NUTRITION: {
                    char nameBuf[100]; GetWindowTextA(hUsername, nameBuf, 100);
                    User u;
                    if (db.loadUser(nameBuf, u)) {
                        std::string msg = "=== PHAC DO DINH DUONG & TAP LUYEN ===\nCho User: " + u.username + "\n\n" + u.getNutritionAdvice();
                        MessageBoxA(hwnd, msg.c_str(), "Tu Van Chuyen Gia", MB_OK | MB_ICONINFORMATION);
                    } else {
                        MessageBoxA(hwnd, "Vui long Tai hoac Luu nguoi dung truoc!", "Thong bao", MB_ICONWARNING);
                    }
                    break;
                }

                // ... (Phần Lưu Bài Tập & Gợi ý giữ nguyên)
                case ID_BTN_SAVE_WORKOUT: {
                    char userBuf[100], exName[100], setBuf[50], repBuf[50], wBuf[50];
                    GetWindowTextA(hUsername, userBuf, 100);
                    if(strlen(userBuf) == 0) { MessageBoxA(hwnd, "Vui long nhap Username!", "Loi", MB_ICONWARNING); break; }
                    GetWindowTextA(hExName, exName, 100); GetWindowTextA(hSets, setBuf, 50); GetWindowTextA(hReps, repBuf, 50); GetWindowTextA(hExWeight, wBuf, 50);
                    try {
                        Workout w; w.username = userBuf; w.name = exName; w.type = "Gym"; w.category = "N/A"; w.difficulty = "Normal";
                        w.sets = std::stoi(setBuf); w.reps = std::stoi(repBuf); w.weight = std::stod(wBuf); w.calculate(); 
                        db.saveWorkout(w);
                        MessageBoxA(hwnd, "Luu bai tap thanh cong vao file!", "Thanh cong", MB_OK);
                    } catch (...) { MessageBoxA(hwnd, "Loi nhap lieu! So phai hop le.", "Loi", MB_ICONERROR); }
                    break;
                }
                
                // TÍNH NĂNG MỚI 2: Tích hợp Thống Kê vào Lịch Sử
                case ID_BTN_VIEW_HISTORY: {
                    char userBuf[100]; GetWindowTextA(hUsername, userBuf, 100);
                    if(strlen(userBuf) == 0) { MessageBoxA(hwnd, "Vui long nhap Username!", "Thong bao", MB_ICONWARNING); break; }
                    
                    std::vector<Workout> history = db.loadHistory(userBuf);
                    if(history.empty()) MessageBoxA(hwnd, "Chua co lich su tap luyen!", "Thong bao", MB_OK);
                    else {
                        double totalVol = 0; int totalSets = 0;
                        std::string msg = "LICH SU TAP LUYEN CUA [" + std::string(userBuf) + "]:\n\n";
                        for(const auto& w : history) {
                            msg += "- " + w.name + ": " + std::to_string(w.sets) + "x" + std::to_string(w.reps) + " (" + std::to_string(w.weight) + "kg)\n";
                            totalVol += w.volume; totalSets += w.sets;
                        }
                        msg += "\n========================\nTHONG KE TONG QUAN:\n";
                        msg += ">> Tong so hiep tap: " + std::to_string(totalSets) + "\n";
                        msg += ">> Tong khoi luong ta (Volume): " + std::to_string(totalVol) + " kg\n";
                        MessageBoxA(hwnd, msg.c_str(), "Bao Cao Luyen Tap", MB_OK);
                    }
                    break;
                }
                
                case ID_BTN_SUGGEST_HOME: {
                    char exName[100]; GetWindowTextA(hExName, exName, 100); std::string exStr(exName);
                    if(exStr.empty()) { MessageBoxA(hwnd, "Vui long nhap Ten bai tap!", "Thong bao", MB_ICONWARNING); break; }
                    std::string inputLower = exStr; for(char& c : inputLower) c = tolower(c);
                    bool found = false;
                    for (const auto& pair : db.homeAlternatives) {
                        std::string keyLower = pair.first; for(char& c : keyLower) c = tolower(c);
                        if (inputLower == keyLower) {
                            std::string suggest = "Bai tap tai nha thay the cho [" + pair.first + "]:\n\n>> " + pair.second;
                            MessageBoxA(hwnd, suggest.c_str(), "Goi y tap luyen", MB_OK); found = true; break;
                        }
                    }
                    if(!found) MessageBoxA(hwnd, "Khong tim thay trong tu dien!", "Thong bao", MB_OK);
                    break;
                }

                // TÍNH NĂNG MỚI 3: Nút Xuất File Excel
                case ID_BTN_EXPORT_CSV: {
                    char userBuf[100]; GetWindowTextA(hUsername, userBuf, 100);
                    if(strlen(userBuf) == 0) { MessageBoxA(hwnd, "Vui long nhap Username de xuat du lieu!", "Thong bao", MB_ICONWARNING); break; }
                    
                    if(db.exportToCSV(userBuf)) {
                        std::string msg = "Da xuat lich su cua [" + std::string(userBuf) + "] ra file Excel thanh cong!\nTen file: " + std::string(userBuf) + "_LichSu.csv";
                        MessageBoxA(hwnd, msg.c_str(), "Xuat Excel Thanh Cong", MB_OK | MB_ICONINFORMATION);
                    } else {
                        MessageBoxA(hwnd, "Loi xuat file! Kiem tra xem ban da co lich su tap chua.", "Loi", MB_ICONERROR);
                    }
                    break;
                }
            }
            break;
        }
        case WM_DESTROY: { PostQuitMessage(0); return 0; }
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "GymAppClass";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    // Mở rộng cửa sổ ra 700x400 để chứa giao diện mới rộng rãi hơn
    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Gym & BMI Tracker - Ban Master",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 400,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); DispatchMessage(&msg);
    }
    return 0;
}