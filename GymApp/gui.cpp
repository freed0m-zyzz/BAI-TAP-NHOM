#include <windows.h>
#include <string>

#include "data.h"

// --- Dinh nghia ID cho cac Control ---
#define ID_TXT_USERNAME 101
#define ID_TXT_HEIGHT 102
#define ID_TXT_WEIGHT 103
#define ID_BTN_LOAD_USER 104
#define ID_BTN_SAVE_USER 105

#define ID_TXT_EX_NAME 201
#define ID_TXT_SETS 202
#define ID_TXT_REPS 203
#define ID_TXT_EX_WEIGHT 204
#define ID_BTN_SAVE_WORKOUT 205

// --- Cac bien Global (Controls & Backend) ---
DataManager db;
HWND hUsername, hHeight, hWeight, hBmiResult;
HWND hExName, hSets, hReps, hExWeight;

// --- Ham xu ly thong diep cua Windows ---
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // --- PHAN 1: USER INFO & BMI ---
            CreateWindowA("STATIC", "THONG TIN NGUOI DUNG & BMI", WS_VISIBLE | WS_CHILD, 20, 20, 250, 20, hwnd, NULL, NULL, NULL);

            CreateWindowA("STATIC", "Ten User:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, NULL, NULL);
            hUsername = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 50, 150, 20, hwnd, (HMENU)ID_TXT_USERNAME, NULL, NULL);

            CreateWindowA("STATIC", "Chieu cao (cm):", WS_VISIBLE | WS_CHILD, 20, 80, 100, 20, hwnd, NULL, NULL, NULL);
            hHeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 80, 130, 20, hwnd, (HMENU)ID_TXT_HEIGHT, NULL, NULL);

            CreateWindowA("STATIC", "Can nang (kg):", WS_VISIBLE | WS_CHILD, 20, 110, 100, 20, hwnd, NULL, NULL, NULL);
            hWeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 110, 130, 20, hwnd, (HMENU)ID_TXT_WEIGHT, NULL, NULL);

            CreateWindowA("BUTTON", "Tai User", WS_VISIBLE | WS_CHILD, 20, 140, 100, 30, hwnd, (HMENU)ID_BTN_LOAD_USER, NULL, NULL);
            CreateWindowA("BUTTON", "Luu & Tinh BMI", WS_VISIBLE | WS_CHILD, 130, 140, 120, 30, hwnd, (HMENU)ID_BTN_SAVE_USER, NULL, NULL);

            hBmiResult = CreateWindowA("STATIC", "BMI: Cho tinh toan...", WS_VISIBLE | WS_CHILD, 20, 180, 300, 20, hwnd, NULL, NULL, NULL);

            // --- PHAN 2: LOG WORKOUT ---
            CreateWindowA("STATIC", "LUU NHAT KY BAI TAP", WS_VISIBLE | WS_CHILD, 350, 20, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindowA("STATIC", "Ten bai tap:", WS_VISIBLE | WS_CHILD, 350, 50, 80, 20, hwnd, NULL, NULL, NULL);
            hExName = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 440, 50, 150, 20, hwnd, (HMENU)ID_TXT_EX_NAME, NULL, NULL);

            CreateWindowA("STATIC", "So Hiep (Sets):", WS_VISIBLE | WS_CHILD, 350, 80, 100, 20, hwnd, NULL, NULL, NULL);
            hSets = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 460, 80, 130, 20, hwnd, (HMENU)ID_TXT_SETS, NULL, NULL);

            CreateWindowA("STATIC", "So Lan (Reps):", WS_VISIBLE | WS_CHILD, 350, 110, 100, 20, hwnd, NULL, NULL, NULL);
            hReps = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 460, 110, 130, 20, hwnd, (HMENU)ID_TXT_REPS, NULL, NULL);

            CreateWindowA("STATIC", "Ta (kg):", WS_VISIBLE | WS_CHILD, 350, 140, 100, 20, hwnd, NULL, NULL, NULL);
            hExWeight = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 460, 140, 130, 20, hwnd, (HMENU)ID_TXT_EX_WEIGHT, NULL, NULL);

            CreateWindowA("BUTTON", "Luu Bai Tap", WS_VISIBLE | WS_CHILD, 350, 170, 240, 30, hwnd, (HMENU)ID_BTN_SAVE_WORKOUT, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // --- XU LY SU KIEN NUT BAM ---
            switch (wmId) {
                case ID_BTN_LOAD_USER: {
                    char nameBuf[100];
                    GetWindowTextA(hUsername, nameBuf, 100);
                    User u;
                    if (db.loadUser(nameBuf, u)) {
                        SetWindowTextA(hHeight, std::to_string(u.height).c_str());
                        SetWindowTextA(hWeight, std::to_string(u.weight).c_str());
                        std::string bmiRes = "BMI: " + std::to_string(u.getBMI()) + " (" + u.getBMICategory() + ")";
                        SetWindowTextA(hBmiResult, bmiRes.c_str());
                        MessageBoxA(hwnd, "Tai user thanh cong!", "Thong bao", MB_OK);
                    } else {
                        MessageBoxA(hwnd, "Khong tim thay nguoi dung nay!", "Loi", MB_ICONERROR);
                    }
                    break;
                }
                case ID_BTN_SAVE_USER: {
                    char nameBuf[100], hBuf[50], wBuf[50];
                    GetWindowTextA(hUsername, nameBuf, 100);
                    GetWindowTextA(hHeight, hBuf, 50);
                    GetWindowTextA(hWeight, wBuf, 50);

                    try {
                        User u;
                        u.username = nameBuf;
                        u.height = std::stod(hBuf);
                        u.weight = std::stod(wBuf);
                        db.saveUser(u);

                        std::string bmiRes = "BMI: " + std::to_string(u.getBMI()) + " (" + u.getBMICategory() + ")";
                        SetWindowTextA(hBmiResult, bmiRes.c_str());
                        MessageBoxA(hwnd, "Da luu nguoi dung va cap nhat file!", "Thanh cong", MB_OK);
                    } catch (...) {
                        MessageBoxA(hwnd, "Loi nhap lieu! Can nang va chieu cao phai la so.", "Loi", MB_ICONERROR);
                    }
                    break;
                }
                case ID_BTN_SAVE_WORKOUT: {
                    char userBuf[100], exName[100], setBuf[50], repBuf[50], wBuf[50];
                    GetWindowTextA(hUsername, userBuf, 100);
                    if(strlen(userBuf) == 0) {
                        MessageBoxA(hwnd, "Vui long nhap Username truoc!", "Loi", MB_ICONWARNING);
                        break;
                    }

                    GetWindowTextA(hExName, exName, 100);
                    GetWindowTextA(hSets, setBuf, 50);
                    GetWindowTextA(hReps, repBuf, 50);
                    GetWindowTextA(hExWeight, wBuf, 50);

                    try {
                        Workout w;
                        w.username = userBuf;
                        w.name = exName;
                        w.type = "Gym"; // Hardcode tam cho demo
                        w.category = "N/A";
                        w.difficulty = "Normal";
                        w.sets = std::stoi(setBuf);
                        w.reps = std::stoi(repBuf);
                        w.weight = std::stod(wBuf);
                        w.calculate(); // Tinh volume

                        db.saveWorkout(w);
                        MessageBoxA(hwnd, "Luu bai tap thanh cong vao file!", "Thanh cong", MB_OK);
                    } catch (...) {
                        MessageBoxA(hwnd, "Loi nhap lieu! Set, Rep va Ta phai la so.", "Loi", MB_ICONERROR);
                    }
                    break;
                }
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

// --- Ham Main cua Win32 ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "GymAppClass";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Gym & BMI Tracker - Win32 API",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 650, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    // Vong lap tin nhan
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
