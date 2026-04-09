#include <windows.h>
#include <string>
#include "data.h" // Nhung code backend

// Dinh nghia ID cho cac nut bam
#define ID_BTN_LOGIN   1
#define ID_BTN_BMI     2
#define ID_BTN_EXPORT  3
#define ID_BTN_SUGGEST 4 // ID moi cho nut Goi y bai tap

// Cac bien toan cuc quan ly giao dien va du lieu
HWND hUsername, hHeight, hWeight, hOutput, hComboExercise;
DataManager db;
User currentUser;
bool isLoggedIn = false;

// Ham tien ich de noi chuoi vao o hien thi ket qua (Output)
void AppendText(HWND hEdit, const std::string& newText) {
    int ndx = GetWindowTextLengthA(hEdit);
    SendMessageA(hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
    SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)newText.c_str());
}

// Ham xu ly cac su kien cua Cua so (Window Procedure)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Tao cac Label va O nhap lieu thong tin User
            CreateWindowExA(0, "STATIC", "Username:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 25, hwnd, NULL, NULL, NULL);
            hUsername = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 110, 20, 150, 25, hwnd, NULL, NULL, NULL);

            CreateWindowExA(0, "STATIC", "Chieu cao(cm):", WS_VISIBLE | WS_CHILD, 20, 60, 90, 25, hwnd, NULL, NULL, NULL);
            hHeight = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_NUMBER, 110, 60, 150, 25, hwnd, NULL, NULL, NULL);

            CreateWindowExA(0, "STATIC", "Can nang(kg):", WS_VISIBLE | WS_CHILD, 20, 100, 90, 25, hwnd, NULL, NULL, NULL);
            hWeight = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD, 110, 100, 150, 25, hwnd, NULL, NULL, NULL);

            // Tao cac Nut bam (Buttons) hang 1
            CreateWindowExA(0, "BUTTON", "Luu / Dang nhap", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 140, 130, 30, hwnd, (HMENU)ID_BTN_LOGIN, NULL, NULL);
            CreateWindowExA(0, "BUTTON", "Xem BMI & Khuyen", WS_VISIBLE | WS_CHILD, 160, 140, 140, 30, hwnd, (HMENU)ID_BTN_BMI, NULL, NULL);
            CreateWindowExA(0, "BUTTON", "Xuat CSV", WS_VISIBLE | WS_CHILD, 310, 140, 100, 30, hwnd, (HMENU)ID_BTN_EXPORT, NULL, NULL);

            // TÍNH NĂNG MỚI: Combobox và Nút cho homeAlternatives (Hang 2)
            CreateWindowExA(0, "STATIC", "Bai tap Gym:", WS_VISIBLE | WS_CHILD, 20, 185, 90, 25, hwnd, NULL, NULL, NULL);
            hComboExercise = CreateWindowExA(0, "COMBOBOX", "", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 110, 182, 150, 150, hwnd, NULL, NULL, NULL);
            CreateWindowExA(0, "BUTTON", "Goi y tap o nha", WS_VISIBLE | WS_CHILD, 270, 180, 140, 30, hwnd, (HMENU)ID_BTN_SUGGEST, NULL, NULL);

            // Do du lieu tu map homeAlternatives vao ComboBox
            for (auto const& pair : db.homeAlternatives) {
                SendMessageA(hComboExercise, CB_ADDSTRING, 0, (LPARAM)pair.first.c_str());
            }
            // Chon mac dinh item dau tien de tranh bi trong
            SendMessageA(hComboExercise, CB_SETCURSEL, 0, 0);

            // Tao o Text box lon de hien thi ket qua (Day xuong duoi mot chut de co cho cho UI moi)
            hOutput = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, 20, 220, 430, 170, hwnd, NULL, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == ID_BTN_LOGIN) {
                char userText[100], heightText[20], weightText[20];
                GetWindowTextA(hUsername, userText, 100);
                GetWindowTextA(hHeight, heightText, 20);
                GetWindowTextA(hWeight, weightText, 20);

                std::string uName(userText);
                if (uName.empty()) {
                    MessageBoxA(hwnd, "Vui long nhap Username!", "Loi", MB_OK | MB_ICONWARNING);
                    break;
                }

                if (db.loadUser(uName, currentUser)) {
                    isLoggedIn = true;
                    SetWindowTextA(hHeight, std::to_string((int)currentUser.height).c_str());
                    SetWindowTextA(hWeight, std::to_string((int)currentUser.weight).c_str());
                    SetWindowTextA(hOutput, ("Dang nhap thanh cong: " + uName + "\r\n").c_str());
                } else {
                    try {
                        currentUser.username = uName;
                        currentUser.height = std::stod(heightText);
                        currentUser.weight = std::stod(weightText);
                        db.saveUser(currentUser);
                        isLoggedIn = true;
                        SetWindowTextA(hOutput, ("Tao moi va luu User thanh cong: " + uName + "\r\n").c_str());
                    } catch (...) {
                        MessageBoxA(hwnd, "Vui long nhap dung so cho Chieu cao va Can nang de tao moi!", "Loi", MB_OK | MB_ICONERROR);
                    }
                }
            }
            else if (LOWORD(wParam) == ID_BTN_BMI) {
                if (!isLoggedIn) {
                    MessageBoxA(hwnd, "Vui long Dang nhap/Luu user truoc!", "Thong bao", MB_OK);
                    break;
                }
                char heightText[20], weightText[20];
                GetWindowTextA(hHeight, heightText, 20);
                GetWindowTextA(hWeight, weightText, 20);
                try {
                    currentUser.height = std::stod(heightText);
                    currentUser.weight = std::stod(weightText);
                } catch(...) {}

                char buffer[256];
                sprintf(buffer, "\r\n--- KET QUA BMI ---\r\nChi so BMI: %.2f\r\nPhan loai: %s\r\n",
                        currentUser.getBMI(), currentUser.getBMICategory().c_str());
                AppendText(hOutput, buffer);

                std::string advice = currentUser.getNutritionAdvice();
                size_t pos = 0;
                while ((pos = advice.find("\n", pos)) != std::string::npos) {
                    advice.replace(pos, 1, "\r\n");
                    pos += 2;
                }
                AppendText(hOutput, advice + "\r\n-------------------\r\n");
            }
            else if (LOWORD(wParam) == ID_BTN_EXPORT) {
                if (!isLoggedIn) {
                    MessageBoxA(hwnd, "Vui long Dang nhap truoc khi xuat file!", "Thong bao", MB_OK);
                    break;
                }
                if (db.exportToCSV(currentUser.username)) {
                    MessageBoxA(hwnd, ("Da xuat thanh cong file: " + currentUser.username + "_LichSu.csv").c_str(), "Thanh cong", MB_OK | MB_ICONINFORMATION);
                    AppendText(hOutput, "Da xuat file CSV thanh cong.\r\n");
                } else {
                    MessageBoxA(hwnd, "Loi: Khong tim thay lich su tap luyen hoac khong the tao file!", "Loi", MB_OK | MB_ICONERROR);
                }
            }
            // XỬ LÝ SỰ KIỆN CHO NÚT MỚI
            else if (LOWORD(wParam) == ID_BTN_SUGGEST) {
                // Lay index cua item dang duoc chon trong ComboBox
                int idx = SendMessageA(hComboExercise, CB_GETCURSEL, 0, 0);
                if (idx != CB_ERR) {
                    char buffer[256];
                    // Lay text cua item do
                    SendMessageA(hComboExercise, CB_GETLBTEXT, idx, (LPARAM)buffer);
                    std::string selectedEx(buffer);

                    // Tra cuu trong backend
                    std::string altEx = db.homeAlternatives[selectedEx];

                    // In ra man hinh ket qua
                    char outBuffer[512];
                    sprintf(outBuffer, "\r\n--- GOI Y TAP TAI NHA ---\r\n>> Bai tap goc: %s\r\n>> Thay the bang: %s\r\n-------------------------\r\n", selectedEx.c_str(), altEx.c_str());
                    AppendText(hOutput, outBuffer);
                } else {
                    MessageBoxA(hwnd, "Vui long chon 1 bai tap truoc!", "Thong bao", MB_OK | MB_ICONWARNING);
                }
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Ham Main cua Win32 API
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "FitnessAppClass";

    WNDCLASSA wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Fitness Tracker & Nutrition - Frontend Win32",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 490, 450,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
