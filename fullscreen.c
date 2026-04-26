#define WIN32_MEAN_AND_LEAN
#include <windows.h>

static RECT old_wnd_rect;

// Fullscreen by maximising without window details
static int fullscreen(HWND hwnd) {
  RECT r;

  LONG_PTR val = GetWindowLongPtr(hwnd, GWL_STYLE);
  if (val & WS_OVERLAPPEDWINDOW) {
    HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);

    MONITORINFO mi = { .cbSize = sizeof(mi) };
    if (!GetMonitorInfo(hmon, &mi)) return 1;
    r = mi.rcMonitor;

    SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    if (!GetWindowRect(hwnd, &old_wnd_rect)) return 1;
  } else {
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    r = old_wnd_rect;
  }

  SetWindowPos(hwnd, NULL, r.left, r.top, r.right - r.left, r.bottom - r.top, 0);
  return 0;
}

// TODO: fullscreen by changing display resolution
// This method was used long time ago for performance reasons, but nowadays it
// is harder to achieve and it is not friendly to users with multiple
// monitors.

static LRESULT window_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
  switch (msg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_KEYDOWN:
      if ('A' == LOWORD(w)) return fullscreen(hwnd);
      break;
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
