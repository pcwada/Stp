#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Screen Tap position Version 1.0\n");
        printf("            Created by Toshihiro Wada.\n");
        printf("Usage:");
        printf("stp [target_area_left] [target_area_top] [target_area_right] [target_area_bottom]\n");
        printf("Sample:\n");
        printf("%s 0 0 200 200\n", argv[0]);
        exit(-1);
    }
    int target_area_left = atoi(argv[1]);
    int target_area_top = atoi(argv[2]);
    int target_area_right = atoi(argv[3]);
    int target_area_bottom = atoi(argv[4]);

    const wchar_t CLASS_NAME[] = L"MyWindowClass";
    const wchar_t WINDOW_NAME[] = L"My Window";

    // ウィンドウクラスの登録
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(1, 0, 0));  // 黒色の背景
    //wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    // ウィンドウの作成
    HWND hwnd = CreateWindowEx(
        //0,
        //WS_EX_LAYERED | WS_EX_TRANSPARENT, // 透明なウィンドウを作成
        WS_EX_LAYERED,
        CLASS_NAME,
        WINDOW_NAME,
        //WS_OVERLAPPEDWINDOW,
        WS_POPUP,
        target_area_left,
        target_area_top,
        target_area_right,
        target_area_bottom,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr);

    if (hwnd == nullptr) {
        return 1;
    }
    SetLayeredWindowAttributes(hwnd, RGB(1, 0, 0), 0, LWA_COLORKEY);
    //SetLayeredWindowAttributes(hwnd, 1, LWA_COLORKEY);
    // ウィンドウの表示
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // ウィンドウを常に手前に表示
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // メッセージループ
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        // ウィンドウが破棄されたらメッセージループを終了
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN: {
        // 左ボタンがクリックされた場合、ウィンドウを閉じる
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        if (x >= 0 && x < 640 && y >= 0 && y < 480) {
            DestroyWindow(hwnd);
        }
        return 0;
    }
    case WM_SETCURSOR:
        // カーソルを変更しないようにする
        SetCursor(LoadCursor(nullptr, IDC_ARROW));
        return 0;
    default:
        // その他のメッセージはデフォルトの処理を呼び出す
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
