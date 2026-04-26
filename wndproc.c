#define WIN32_MEAN_AND_LEAN
#include <windows.h>

static LRESULT window_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
  switch (msg) {
    // Required if we want another thread sending "plz exit" messages
    //case WM_CLOSE:
    //  DestroyWindow(hwnd);
    //  return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, msg, w, l);
}

// We can use hprev to communicate with other running instances
int WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR cmdline, int show) {
  WNDCLASSEX wcex = {
    .cbSize        = sizeof(WNDCLASSEX),
    .style         = CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc   = window_proc,
    .hInstance     = hinst,
    .hCursor       = LoadCursor(NULL, IDC_ARROW),
    .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
    .lpszClassName = "poc-window-class",
  };
  if (!RegisterClassEx(&wcex)) {
    MessageBox(NULL, "Failed to register window class", "Error", 0);
    return 1;
  }

  HWND hwnd = CreateWindow(
      "poc-window-class",
      "Window Title",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      300, 200, // Window size
      NULL, NULL,
      hinst,
      NULL);
  if (!hwnd) {
    MessageBox(NULL, "Failed to create window", "Error", 0);
    return 1;
  }

  ShowWindow(hwnd, show);
  UpdateWindow(hwnd);

  MSG msg;
  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
