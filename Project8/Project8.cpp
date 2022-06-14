#include "Project8.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"Draw Groups Of Connected Shapes App";        
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
TCHAR px1[MAX_STRING], py1[MAX_STRING], px2[MAX_STRING], py2[MAX_STRING], px3[MAX_STRING], py3[MAX_STRING], poly[222];
HMENU hMenu;
vector <vector<int>> group;
int colorR[MAXSHAPE] = { 0 }, colorG[MAXSHAPE] = { 0 }, colorB[MAXSHAPE] = { 0 };
int ix1 = 0, iy1 = 0, ix2 = 0, iy2 = 0, iy3 = 0, ix3 = 0;
bool ICheck = 1;
vector <Geometry *> f;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void addMenu(HWND hWnd);
set<int> checkBelong();
bool isNum(TCHAR p[]);

INT_PTR CALLBACK InputHinhVuong(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK InputHinhChuNhat(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK InputHinhTamGiac(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK InputHinhTron(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK InputHinhEllipse(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK InputHinhBanNguyet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK InputHinhPolygon(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{   
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    srand(time(NULL));
    FILE* stream;
    freopen_s(&stream, "UserInputLog.txt", "w", stdout);
    cout << "-- USER INPUT LOG --\n\n";
    fclose(stdout);
    LoadStringW(hInstance, IDC_PROJECT8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT8));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
wcex.lpfnWndProc = WndProc;
wcex.cbClsExtra = 0;
wcex.cbWndExtra = 0;
wcex.hInstance = hInstance;
wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_PROJECT8));
wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECT8);
wcex.lpszClassName = szWindowClass;
wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        10, 10, dwWidth - 10, dwHeight - 10, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

set<int> checkBelong() {
    int n = f.size();
    set<int> belong;
    for (int i = 0; i < n - 1; i++) {
        bool ok = 0;
        for (auto u : f[n - 1]->VLinearEqua) {
            if (ok)
                break;
            for (auto v : f[i]->VLinearEqua) {
                if (CheckSolution(u, v)) {
                    belong.insert(i);
                    ok = 1;
                    break;
                }
            }
        }
        if (ok)
            continue;
        for (auto u : f[n - 1]->VLinearEqua) {
            if (ok)
                break;
            for (auto v : f[i]->VQuadraEqua) {
                if (CheckSolution(u, v)) {
                    belong.insert(i);
                    ok = 1;
                    break;
                }
            }
        }
        if (ok)
            continue;
        for (auto u : f[n - 1]->VQuadraEqua) {
            if (ok)
                break;
            for (auto v : f[i]->VLinearEqua) {
                if (CheckSolution(v, u)) {
                    belong.insert(i);
                    ok = 1;
                    break;
                }
            }
        }
        if (ok)
            continue;
        for (auto u : f[n - 1]->VQuadraEqua) {
            if (ok)
                break;
            for (auto v : f[i]->VQuadraEqua) {
                if (CheckSolution(v, u)) {
                    belong.insert(i);
                    ok = 1;
                    break;
                }
            }
        }
        if (!ok) {
            if (f[n - 1]->IsInside(f[i]->getCentroid()) || f[i]->IsInside(f[n - 1]->getCentroid())) {
                belong.insert(i);
                cout << "\nInside " << i << endl;
            }
        }
    }
    return belong;
}
int m = 0;
void groupShape(set <int> belong) {
    int n = f.size();
    if (belong.size() == 0 and f.size() > m) {
        m = f.size();
        group.push_back({ n - 1 });
        int R, G, B;
        int limitColor = 180;
        R = rand() % 256, G = rand() % 256, B = rand() % 256;
        while (R >= limitColor and G >= limitColor and B >= limitColor) {
            R = rand() % 256, G = rand() % 256, B = rand() % 256;
        }
        colorR[n - 1] = R;
        colorG[n - 1] = G;
        colorB[n - 1] = B;
    }

    else if (f.size() > m) {
        m = f.size();
        vector <int> sGroup;
        for (int i = group.size() - 1; i >= 0; i--) {
            for (int j = 0; j < group[i].size(); j++) {
                if (belong.count(group[i][j])) {
                    sGroup.push_back(i);
                    break;
                }
            }
        }
        cout << "\nMerge groups: ";
        for (int i = 0; i < sGroup.size() - 1; i++) {
            cout << sGroup[i] << ' ';
            for (auto item : group[sGroup[i]]) {
                group[sGroup[sGroup.size() - 1]].push_back(item);
                colorR[item] = colorR[group[sGroup[sGroup.size() - 1]][0]];
                colorG[item] = colorG[group[sGroup[sGroup.size() - 1]][0]];
                colorB[item] = colorB[group[sGroup[sGroup.size() - 1]][0]];
            }
            group.erase(group.begin() + sGroup[i]);
        }
        group[sGroup[sGroup.size() - 1]].push_back(n - 1);
        colorR[n - 1] = colorR[group[sGroup[sGroup.size() - 1]][0]];
        colorG[n - 1] = colorG[group[sGroup[sGroup.size() - 1]][0]];
        colorB[n - 1] = colorB[group[sGroup[sGroup.size() - 1]][0]];
        cout << "To Group << " << sGroup[sGroup.size() - 1];
    }
}
void debugLog();
void debugLog() {
    FILE* stream;
    freopen_s(&stream, "UserInputLog.txt", "a", stdout);
    wstring test(&poly[0]);
    string s(test.begin(), test.end());
    cout << ix1 << ' ' << iy1 << ' ' << ix2 << ' ' << iy2 << ' ' << ix3 << ' ' << iy3 << ' ' << s << '\n';
    fclose(stdout);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_CREATE:
        addMenu(hWnd);
        break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 220:
            //MessageBeep(MB_OK);
            //MessageBox(NULL, L"Error Registering Window Class", L"Error Registering Window", MB_OK | MB_ICONEXCLAMATION);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX1), hWnd, InputHinhVuong);
            if (ICheck)
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 221:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX2), hWnd, InputHinhChuNhat);
            if (ICheck)
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 222:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX3), hWnd, InputHinhTamGiac);
            if (ICheck)
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 223:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX4), hWnd, InputHinhTron);
            if (ICheck)
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 224:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX5), hWnd, InputHinhEllipse);
            if (ICheck)
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 225:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX6), hWnd, InputHinhBanNguyet);
            if (ICheck)
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 226:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX7), hWnd, InputHinhPolygon);
            if (ICheck)
                InvalidateRect(hWnd, NULL, TRUE);
           
            break;
        case IDM_ABOUT:

            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        if (!f.size()) {
            EndPaint(hWnd, &ps);
            return 0;
        }
        debugLog();
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        FILE* stream;
        freopen_s(&stream, "CurrentProcessingLog.txt", "w", stdout);
        int n = f.size();
        set <int> belong = checkBelong();
        cout << "CONNECT: ";
        for (auto i : belong) {
            cout << i << ' ';
        }
        groupShape(belong);
        cout << "\nCURRENT GROUP SIZE: " << group.size();
        vector <float> groupArea;
        float sum = 0;
        for (int i = 0; i < group.size(); i++) {
            sum = 0;
            for (auto shape : group[i]) {
                sum += f[shape]->getArea();
            }
            groupArea.push_back(sum);
            string num = to_string(sum);
            string ss = "";
            int index = 0;
            while (index < num.length() and num[index] != '.') {
                ss += num[index];
                index++;
            }
            for (int i = 0; i < 3; i++) {
                ss += num[index + i];
            }
            string res = to_string(i) + ":  " + ss;
            TCHAR* tmp = new TCHAR[res.length()];
            tmp[res.size()] = 0;
            copy(res.begin(), res.end(), tmp);
            DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN);
            WORD dwWidth = GetSystemMetrics(SM_CXSCREEN);
            int limitHeight = 15;
            RECT rleft = { dwWidth - 200, i * limitHeight, dwWidth, i * limitHeight + limitHeight };
            RECT r = { dwWidth - 150, i * limitHeight, dwWidth, i * limitHeight + limitHeight };
            DrawText(hdc, L"Nhóm ", 8, &rleft, (DT_LEFT) | (DT_SINGLELINE));
            DrawText(hdc, tmp, res.length(), &r, (DT_LEFT) | (DT_SINGLELINE));
        }
        cout << endl;
        fclose(stdout);

        for (int i = 0; i < f.size(); i++) {
            SelectObject(hdc, CreateHatchBrush(HS_API_MAX, RGB(255, 0, 250)));
            SelectObject(hdc, CreatePen(PS_INSIDEFRAME, 3, RGB(colorR[i], colorG[i], colorB[i])));
            f[i]->fill(hdc, colorR[i], colorG[i], colorB[i]);
            f[i]->draw(hdc);
        }

        for (int i = 0; i < group.size(); i++) {
            CPoint P = f[group[i][0]]->getCentroid();
            int x = P.getX();
            int y = P.getY();
            std::string res = std::to_string(i);
            TCHAR* tmp = new TCHAR[res.length()];
            copy(res.begin(), res.end(), tmp);
            RECT r = { x,  y, x + 100, y + 15 };
            DrawText(hdc, tmp, res.length(), &r, (DT_LEFT) | (DT_SINGLELINE));
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool isNum(TCHAR p[]) {
    wstring test(&p[0]);
    string s(test.begin(), test.end());
    if (s[0] == '-') {
        if (s.length() >= 2) {
            if (s[1] < '0' || s[1] > '9') {
                return 0;
            }
        }
        else {
            return 0;
        }
    }
    int d = (s[0] == '-');
    for (int i = d; i < s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            return 0;
        }
    }
    return 1;
}

INT_PTR CALLBACK InputHinhVuong(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            ix1 = 0; iy1 = 0; ix2 = 0; iy2 = 0; ix3 = 0; iy3 = 0;
            bool ok = 1;
            ICheck = 1;
            GetDlgItemText(hDlg, IDC_EDIT1, px1, 30 - 1);
            if (!isNum(px1))
                ok = 0;
            ix1 = _ttoi(px1);
            GetDlgItemText(hDlg, IDC_EDIT2, py1, 30 - 1);
            if (!isNum(py1))
                ok = 0;
            iy1 = _ttoi(py1);
            GetDlgItemText(hDlg, IDC_EDIT3, px2, 30 - 1);
            if (!isNum(px2))
                ok = 0;
            ix2 = _ttoi(px2);
            GetDlgItemText(hDlg, IDC_EDIT4, py2, 30 - 1);
            if (!isNum(py2))
                ok = 0;
            iy2 = _ttoi(py2);
            if (ok and ix1 < ix2 and iy1 < iy2 and abs(ix1 - ix2) == abs(iy1 - iy2)) {
                Geometry* FF = new CRectangle(CPoint(ix1, iy1), CPoint(ix2, iy2));
                f.push_back(FF);
            }
            else {
                ICheck = 0;
                MessageBox(hDlg, L"Toạ độ 2 điểm không tạo được hình vuông", L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK InputHinhChuNhat(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            ix1 = 0; iy1 = 0; ix2 = 0; iy2 = 0; ix3 = 0; iy3 = 0;
            ICheck = 1;
            bool ok = 1;
            GetDlgItemText(hDlg, IDC_EDIT1, px1, 30 - 1);
            if (!isNum(px1))
                ok = 0;
            ix1 = _ttoi(px1);

            GetDlgItemText(hDlg, IDC_EDIT2, py1, 30 - 1);
            if (!isNum(py1))
                ok = 0;
            iy1 = _ttoi(py1);

            GetDlgItemText(hDlg, IDC_EDIT3, px2, 30 - 1);
            if (!isNum(px2))
                ok = 0;
            ix2 = _ttoi(px2);

            GetDlgItemText(hDlg, IDC_EDIT4, py2, 30 - 1);
            if (!isNum(py2))
                ok = 0;
            iy2 = _ttoi(py2);

            if (ok and ix1 < ix2 and iy1 < iy2) {
                Geometry* FF = new CRectangle(CPoint(ix1, iy1), CPoint(ix2, iy2));
                f.push_back(FF);
               
            }
            else {
                ICheck = 0;
                MessageBox(hDlg, L"Toạ độ 2 điểm không tạo được hình chữ nhật", L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK InputHinhTamGiac(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            ix1 = 0; iy1 = 0; ix2 = 0; iy2 = 0; ix3 = 0; iy3 = 0;
            ICheck = 1;
            bool ok = 1;
            GetDlgItemText(hDlg, IDC_EDIT1, px1, 30 - 1);
            if (!isNum(px1))
                ok = 0;
            ix1 = _ttoi(px1);
            GetDlgItemText(hDlg, IDC_EDIT2, py1, 30 - 1);
            if (!isNum(py1))
                ok = 0;
            iy1 = _ttoi(py1);
            GetDlgItemText(hDlg, IDC_EDIT3, px2, 30 - 1);
            if (!isNum(px2))
                ok = 0;
            ix2 = _ttoi(px2);
            GetDlgItemText(hDlg, IDC_EDIT4, py2, 30 - 1);
            if (!isNum(py2))
                ok = 0;
            GetDlgItemText(hDlg, IDC_EDIT5, px3, 30 - 1);
            if (!isNum(px3))
            ok = 0;
            ix3 = _ttoi(px3);
            GetDlgItemText(hDlg, IDC_EDIT6, py3, 30 - 1);
            if (!isNum(py3))
            ok = 0;
            iy3 = _ttoi(py3);
            vector <CPoint> V;
            V.push_back(CPoint(ix1, iy1));
            V.push_back(CPoint(ix2, iy2));
            V.push_back(CPoint(ix3, iy3));
            float dis[3];
            dis[0] = Distant(V[0], V[1]);
            dis[1] = Distant(V[1], V[2]);
            dis[2] = Distant(V[2], V[0]);
            std::sort(dis, dis + 3);
            if (dis[2] != dis[1] + dis[0]) {
                Geometry* FF = new CTriangle(V);
                f.push_back(FF);
            }
            else {
                ICheck = 0;
                MessageBox(hDlg, L"Toạ độ 3 điểm không tạo được tam giác", L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK InputHinhTron(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            ix1 = 0; iy1 = 0; ix2 = 0; iy2 = 0; ix3 = 0; iy3 = 0;
            bool ok = 1;
            ICheck = 1;
            GetDlgItemText(hDlg, IDC_EDIT1, px1, 30 - 1);
            if (!isNum(px1))
            ok = 0;
            ix1 = _ttoi(px1);
            GetDlgItemText(hDlg, IDC_EDIT2, py1, 30 - 1);
            if (!isNum(py1))
            ok = 0;
            iy1 = _ttoi(py1);
            GetDlgItemText(hDlg, IDC_EDIT3, px2, 30 - 1);
            if (!isNum(px2))
            ok = 0;
            ix2 = _ttoi(px2);
         
            if (ix2 > 0 and ok) {
                Geometry* FF = new CCircle(CPoint(ix1, iy1), ix2);
                f.push_back(FF);
            }
            else {
                ICheck = 0;
                MessageBox(hDlg, L"Không tạo được hình tròn", L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
            }

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK InputHinhEllipse(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            ix1 = 0; iy1 = 0; ix2 = 0; iy2 = 0; ix3 = 0; iy3 = 0;
            bool ok = 1;
            ICheck = 1;
            GetDlgItemText(hDlg, IDC_EDIT1, px1, 30 - 1);
            if (!isNum(px1))
            ok = 0;
            ix1 = _ttoi(px1);
            GetDlgItemText(hDlg, IDC_EDIT2, py1, 30 - 1);
            if (!isNum(py1))
            ok = 0;
            iy1 = _ttoi(py1);
            GetDlgItemText(hDlg, IDC_EDIT3, px2, 30 - 1);
            if (!isNum(px2))
            ok = 0;
            ix2 = _ttoi(px2);
            GetDlgItemText(hDlg, IDC_EDIT4, py2, 30 - 1);
            if (!isNum(py2))
            ok = 0;
            iy2 = _ttoi(py2);
            if (!isNum(py2))
            ok = 0;
            if (ix2 > 0 and iy2 > 0 and ok) {
                Geometry* FF = new CElipse(CPoint(ix1, iy1), ix2, iy2);
                f.push_back(FF);
            }
            else {
                ICheck = 0;
                MessageBox(hDlg, L"Không tạo được ellipse", L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
            }

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK InputHinhBanNguyet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            ix1 = 0; iy1 = 0; ix2 = 0; iy2 = 0; ix3 = 0; iy3 = 0;
            bool ok = 1;
            ICheck = 1;
            GetDlgItemText(hDlg, IDC_EDIT1, px1, 30 - 1);
            if (!isNum(px1))
            ok = 0;
            ix1 = _ttoi(px1);
            GetDlgItemText(hDlg, IDC_EDIT2, py1, 30 - 1);
            if (!isNum(py1))
            ok = 0;
            iy1 = _ttoi(py1);
            GetDlgItemText(hDlg, IDC_EDIT3, px2, 30 - 1);
            if (!isNum(px2))
            ok = 0;
            ix2 = _ttoi(px2);
            UINT u = IsDlgButtonChecked(hDlg, IDC_CHECK1);
            if (!u)
                u = -1;
            iy2 = _ttoi(py2);
            
            if (ix2 > 0 and ok) {
                Geometry* FF = new CSemiCirlce(CPoint(ix1, iy1), ix2, u);
                f.push_back(FF);
            }
            else {
                ICheck = 0;
                MessageBox(hDlg, L"Không tạo được hình bán nguyệt", L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK InputHinhPolygon (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            ix1 = 0; iy1 = 0; ix2 = 0; iy2 = 0; ix3 = 0; iy3 = 0;
            ICheck = 1;
            GetDlgItemText(hDlg, IDC_EDIT3, poly , 222 - 1);
            wstring test(&poly[0]);
            string s(test.begin(), test.end());
            s = '#' + s + '#';
            int digit = 0;
            vector <int> d;
            for (int i = 1; i < s.length(); i++) {
                if (isdigit(s[i])) {
                    digit = digit * 10 + s[i] - 48;
                }
                else {
                    if (isdigit(s[i - 1]))
                        d.push_back(digit);
                    digit = 0;
                }
            }
            vector <CPoint> V;
            for (int i = 0; i < d.size() / 2; i++) {
                V.push_back(CPoint(d[i * 2], d[i * 2 + 1]));
            }
            if (V.size() >= 3 and isConvex(V)) {
                Geometry* FF = new CPolygon(V);
                f.push_back(FF);
            }
            else {
                ICheck = 0;
                MessageBox(hDlg, L"Toạ độ các điểm không hợp lệ", L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
            }
            
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void addMenu(HWND hWnd) {
    hMenu = CreateMenu();
    HMENU menuCon = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuCon, L"Chọn hình");
    AppendMenu(menuCon, MF_STRING, 220, L"Hình Vuông");
    AppendMenu(menuCon, MF_STRING, 221, L"Hình Chữ Nhật");
    AppendMenu(menuCon, MF_STRING, 222, L"Hình Tam Giác");
    AppendMenu(menuCon, MF_STRING, 223, L"Hình Tròn");
    AppendMenu(menuCon, MF_STRING, 224, L"Hình Ellipse");
    AppendMenu(menuCon, MF_STRING, 225, L"Hình Bán Nguyệt");
    AppendMenu(menuCon, MF_STRING, 226, L"Hình Đa Giác Lồi");
    AppendMenu(hMenu, MF_STRING, IDM_ABOUT, L"About");
    SetMenu(hWnd, hMenu);
}
