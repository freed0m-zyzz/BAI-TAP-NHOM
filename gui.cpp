#include <windows.h>
#include <string>

// Goi file data.cpp
#include "data.cpp"

#define ID_TXT_USERNAME 101
#define ID_TXT_HEIGHT 102
#define ID_TXT_WEIGHT 103
#define ID_TXT_AGE 104        
#define ID_CB_GENDER 105      
#define ID_BTN_LOAD_USER 106
#define ID_BTN_SAVE_USER 107
#define ID_BTN_NUTRITION 108 

#define ID_CB_GOAL 109 
#define ID_BTN_TRAINING_ADVICE 110 
#define ID_CB_ACTIVITY 111    
#define ID_BTN_CALC_CALO 112  

#define ID_TXT_EX_NAME 201
#define ID_TXT_SETS 202
#define ID_TXT_REPS 203
#define ID_TXT_EX_WEIGHT 204
#define ID_BTN_SAVE_WORKOUT 205
#define ID_BTN_VIEW_HISTORY 206
#define ID_BTN_SUGGEST_HOME 207
#define ID_BTN_EXPORT_CSV 208 

DataManager db;
HWND hUsername, hHeight, hWeight, hAge, hGenderCb, hBmiResult, hGoalCb, hActivityCb;
HWND hExName, hSets, hReps, hExWeight;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // --- USER INFO & BODY STATS ---
            CreateWindowA("STATIC", "THONG TIN NGUOI DUNG", WS_VISIBLE | WS_CHILD, 20, 20, 250, 20, hwnd, NULL, NULL, NULL);
            CreateWindowA("STATIC", "Ten User:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, NULL, NULL);
            hUsername = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 50, 150, 20, hwnd, (HMENU)ID_TXT_USERNAME, NULL, NULL);
            
            CreateWindowA("STATIC", "Chieu cao (cm):", WS_VISIBLE | WS_CHILD, 20, 80, 100, 20, hwnd, NULL, NULL, NULL);
            hHeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 80, 130, 20, hwnd, (HMENU)ID_TXT_HEIGHT, NULL, NULL);
            
            CreateWindowA("STATIC", "Can nang (kg):", WS_VISIBLE | WS_CHILD, 20, 110, 100, 20, hwnd, NULL, NULL, NULL);
            hWeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 110, 130, 20, hwnd, (HMENU)ID_TXT_WEIGHT, NULL, NULL);

            CreateWindowA("STATIC", "Tuoi:", WS_VISIBLE | WS_CHILD, 20, 140, 40, 20, hwnd, NULL, NULL, NULL);
            hAge = CreateWindowA("EDIT", "20", WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 140, 40, 20, hwnd, (HMENU)ID_TXT_AGE, NULL, NULL);
            CreateWindowA("STATIC", "Gioi tinh:", WS_VISIBLE | WS_CHILD, 115, 140, 60, 20, hwnd, NULL, NULL, NULL);
            hGenderCb = CreateWindowA("COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE, 180, 135, 70, 100, hwnd, (HMENU)ID_CB_GENDER, NULL, NULL);
            SendMessageA(hGenderCb, CB_ADDSTRING, 0, (LPARAM)"Nam");
            SendMessageA(hGenderCb, CB_ADDSTRING, 0, (LPARAM)"Nu");
            SendMessageA(hGenderCb, CB_SETCURSEL, 0, 0);
            
            CreateWindowA("BUTTON", "Tai", WS_VISIBLE | WS_CHILD, 20, 170, 50, 30, hwnd, (HMENU)ID_BTN_LOAD_USER, NULL, NULL);
            CreateWindowA("BUTTON", "Luu & Tinh BMI", WS_VISIBLE | WS_CHILD, 80, 170, 110, 30, hwnd, (HMENU)ID_BTN_SAVE_USER, NULL, NULL);
            CreateWindowA("BUTTON", "T/V Dinh Duong", WS_VISIBLE | WS_CHILD, 200, 170, 110, 30, hwnd, (HMENU)ID_BTN_NUTRITION, NULL, NULL);
            
            hBmiResult = CreateWindowA("STATIC", "BMI: Cho tinh toan...", WS_VISIBLE | WS_CHILD, 20, 210, 300, 20, hwnd, NULL, NULL, NULL);

            CreateWindowA("STATIC", "Muc do hoat dong:", WS_VISIBLE | WS_CHILD, 20, 240, 120, 20, hwnd, NULL, NULL, NULL);
            hActivityCb = CreateWindowA("COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE, 140, 235, 170, 150, hwnd, (HMENU)ID_CB_ACTIVITY, NULL, NULL);
            SendMessageA(hActivityCb, CB_ADDSTRING, 0, (LPARAM)"Van phong (It van dong)");
            SendMessageA(hActivityCb, CB_ADDSTRING, 0, (LPARAM)"Nhe (Tap 1-3 ngay/tuan)");
            SendMessageA(hActivityCb, CB_ADDSTRING, 0, (LPARAM)"Vua (Tap 3-5 ngay/tuan)");
            SendMessageA(hActivityCb, CB_ADDSTRING, 0, (LPARAM)"Nang (Tap 6-7 ngay/tuan)");
            SendMessageA(hActivityCb, CB_SETCURSEL, 0, 0);

            CreateWindowA("BUTTON", "Tinh Nhu Cau Calo (TDEE)", WS_VISIBLE | WS_CHILD, 20, 265, 290, 30, hwnd, (HMENU)ID_BTN_CALC_CALO, NULL, NULL);

            CreateWindowA("STATIC", "Chon Muc Tieu:", WS_VISIBLE | WS_CHILD, 20, 310, 100, 20, hwnd, NULL, NULL, NULL);
            hGoalCb = CreateWindowA("COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE, 130, 305, 180, 100, hwnd, (HMENU)ID_CB_GOAL, NULL, NULL);
            SendMessageA(hGoalCb, CB_ADDSTRING, 0, (LPARAM)"Hypertrophy (Tang co)");
            SendMessageA(hGoalCb, CB_ADDSTRING, 0, (LPARAM)"Strength (Tang suc manh)");
            SendMessageA(hGoalCb, CB_ADDSTRING, 0, (LPARAM)"Endurance (Tang suc ben)");
            SendMessageA(hGoalCb, CB_SETCURSEL, 0, 0); 
            CreateWindowA("BUTTON", "Xuat Phac Do Tap Luyen", WS_VISIBLE | WS_CHILD, 20, 335, 290, 30, hwnd, (HMENU)ID_BTN_TRAINING_ADVICE, NULL, NULL);

            // --- WORKOUT LOG --- 
            CreateWindowA("STATIC", "LUU NHAT KY BAI TAP", WS_VISIBLE | WS_CHILD, 390, 20, 200, 20, hwnd, NULL, NULL, NULL);
            CreateWindowA("STATIC", "Ten bai tap:", WS_VISIBLE | WS_CHILD, 390, 50, 80, 20, hwnd, NULL, NULL, NULL);
            hExName = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 480, 50, 150, 20, hwnd, (HMENU)ID_TXT_EX_NAME, NULL, NULL);
            CreateWindowA("STATIC", "So Hiep (Sets):", WS_VISIBLE | WS_CHILD, 390, 80, 100, 20, hwnd, NULL, NULL, NULL);
            hSets = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 500, 80, 130, 20, hwnd, (HMENU)ID_TXT_SETS, NULL, NULL);
            CreateWindowA("STATIC", "So Lan (Reps):", WS_VISIBLE | WS_CHILD, 390, 110, 100, 20, hwnd, NULL, NULL, NULL);
            hReps = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 500, 110, 130, 20, hwnd, (HMENU)ID_TXT_REPS, NULL, NULL);
            CreateWindowA("STATIC", "Ta (kg):", WS_VISIBLE | WS_CHILD, 390, 140, 100, 20, hwnd, NULL, NULL, NULL);
            hExWeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 500, 140, 130, 20, hwnd, (HMENU)ID_TXT_EX_WEIGHT, NULL, NULL);
            
            CreateWindowA("BUTTON", "Luu Bai Tap", WS_VISIBLE | WS_CHILD, 390, 170, 240, 30, hwnd, (HMENU)ID_BTN_SAVE_WORKOUT, NULL, NULL);
            CreateWindowA("BUTTON", "Xem Lich Su", WS_VISIBLE | WS_CHILD, 390, 210, 110, 30, hwnd, (HMENU)ID_BTN_VIEW_HISTORY, NULL, NULL);
            CreateWindowA("BUTTON", "Goi Y Tap Nha", WS_VISIBLE | WS_CHILD, 520, 210, 110, 30, hwnd, (HMENU)ID_BTN_SUGGEST_HOME, NULL, NULL);
            CreateWindowA("BUTTON", "Xuat du lieu ra Excel (.csv)", WS_VISIBLE | WS_CHILD, 390, 250, 240, 30, hwnd, (HMENU)ID_BTN_EXPORT_CSV, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case ID_BTN_LOAD_USER: {
                    char nameBuf[100]; GetWindowTextA(hUsername, nameBuf, 100);
                    User u;
                    if (db.loadUser(nameBuf, u)) {
                        SetWindowTextA(hHeight, std::to_string(u.height).c_str());
                        SetWindowTextA(hWeight, std::to_string(u.weight).c_str());
                        SetWindowTextA(hAge, std::to_string(u.age).c_str()); 
                        if(u.gender == "Nu") SendMessageA(hGenderCb, CB_SETCURSEL, 1, 0); 
                        else SendMessageA(hGenderCb, CB_SETCURSEL, 0, 0);

                        std::string bmiRes = "BMI: " + std::to_string(u.getBMI()) + " (" + u.getBMICategory() + ")";
                        SetWindowTextA(hBmiResult, bmiRes.c_str());
                        MessageBoxA(hwnd, "Tai user thanh cong!", "Thong bao", MB_OK);
                    } else MessageBoxA(hwnd, "Khong tim thay nguoi dung nay!", "Loi", MB_ICONERROR);
                    break;
                }
                case ID_BTN_SAVE_USER: {
                    char nameBuf[100], hBuf[50], wBuf[50], ageBuf[20], genBuf[20];
                    GetWindowTextA(hUsername, nameBuf, 100); GetWindowTextA(hHeight, hBuf, 50); GetWindowTextA(hWeight, wBuf, 50);
                    GetWindowTextA(hAge, ageBuf, 20);
                    int gIdx = SendMessageA(hGenderCb, CB_GETCURSEL, 0, 0);
                    SendMessageA(hGenderCb, CB_GETLBTEXT, gIdx, (LPARAM)genBuf);

                    if (strlen(nameBuf) == 0 || strlen(hBuf) == 0 || strlen(wBuf) == 0 || strlen(ageBuf) == 0) {
                        MessageBoxA(hwnd, "Vui long nhap day du thong tin!", "Thong bao", MB_ICONWARNING); break;
                    }
                    try {
                        User u; u.username = nameBuf; u.height = std::stod(hBuf); u.weight = std::stod(wBuf);
                        u.age = std::stoi(ageBuf); u.gender = genBuf;
                        if (db.saveUser(u)) {
                            std::string bmiRes = "BMI: " + std::to_string(u.getBMI()) + " (" + u.getBMICategory() + ")";
                            SetWindowTextA(hBmiResult, bmiRes.c_str());
                            MessageBoxA(hwnd, "Luu thanh cong vao users.txt!", "Thanh cong", MB_OK);
                        } else MessageBoxA(hwnd, "Khong the mo file users.txt!", "Loi", MB_ICONERROR);
                    } catch (...) { MessageBoxA(hwnd, "Loi dinh dang so!", "Loi", MB_ICONERROR); }
                    break;
                }
                case ID_BTN_CALC_CALO: {
                    char nameBuf[100]; GetWindowTextA(hUsername, nameBuf, 100);
                    User u;
                    if (db.loadUser(nameBuf, u)) {
                        int actIdx = SendMessageA(hActivityCb, CB_GETCURSEL, 0, 0);
                        double mult = 1.2; 
                        if(actIdx == 1) mult = 1.375; 
                        if(actIdx == 2) mult = 1.55;  
                        if(actIdx == 3) mult = 1.725; 

                        double tdee = u.getTDEE(mult);
                        std::string msg = "=== BAO CAO NHU CAU NANG LUONG ===\nNguoi dung: " + u.username + "\n\n";
                        msg += "BMR (Trao doi chat co ban): " + std::to_string((int)u.getBMR()) + " kcal\n";
                        msg += "TDEE (Tong nang luong tieu hao): " + std::to_string((int)tdee) + " kcal/ngay\n\n";
                        msg += ">>> MUC TIEU CUA BAN <<<\n";
                        msg += "- De GIAM CAN: " + std::to_string((int)(tdee - 500)) + " kcal/ngay\n";
                        msg += "- De DUY TRI: " + std::to_string((int)tdee) + " kcal/ngay\n";
                        msg += "- De TANG CO: " + std::to_string((int)(tdee + 300)) + " kcal/ngay\n";
                        
                        MessageBoxA(hwnd, msg.c_str(), "Tinh Toan Calo (TDEE)", MB_OK | MB_ICONINFORMATION);
                    } else MessageBoxA(hwnd, "Vui long Tai hoac Luu nguoi dung truoc!", "Thong bao", MB_ICONWARNING);
                    break;
                }
                case ID_BTN_NUTRITION: {
                    char nameBuf[100]; GetWindowTextA(hUsername, nameBuf, 100);
                    User u;
                    if (db.loadUser(nameBuf, u)) {
                        std::string msg = "=== PHAC DO DINH DUONG & TAP LUYEN ===\nCho User: " + u.username + "\n\n" + u.getNutritionAdvice();
                        MessageBoxA(hwnd, msg.c_str(), "Tu Van Chuyen Gia", MB_OK | MB_ICONINFORMATION);
                    } else MessageBoxA(hwnd, "Vui long Tai hoac Luu nguoi dung truoc!", "Thong bao", MB_ICONWARNING);
                    break;
                }
                case ID_BTN_TRAINING_ADVICE: {
                    int index = SendMessageA(hGoalCb, CB_GETCURSEL, 0, 0); char goalBuf[100];
                    SendMessageA(hGoalCb, CB_GETLBTEXT, index, (LPARAM)goalBuf);
                    std::string advice = db.getTrainingAdvice(goalBuf);
                    MessageBoxA(hwnd, advice.c_str(), "Phac Do Tap Luyen", MB_OK | MB_ICONINFORMATION);
                    break;
                }
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
                case ID_BTN_EXPORT_CSV: {
                    char userBuf[100]; GetWindowTextA(hUsername, userBuf, 100);
                    if(strlen(userBuf) == 0) { MessageBoxA(hwnd, "Vui long nhap Username de xuat du lieu!", "Thong bao", MB_ICONWARNING); break; }
                    if(db.exportToCSV(userBuf)) {
                        std::string msg = "Da xuat lich su cua [" + std::string(userBuf) + "] ra file Excel thanh cong!\nTen file: " + std::string(userBuf) + "_LichSu.csv";
                        MessageBoxA(hwnd, msg.c_str(), "Xuat Excel Thanh Cong", MB_OK | MB_ICONINFORMATION);
                    } else MessageBoxA(hwnd, "Loi xuat file! Kiem tra xem ban da co lich su tap chua.", "Loi", MB_ICONERROR);
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

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Gym & BMI Tracker - Ban Ultimate Virtual PT",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
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