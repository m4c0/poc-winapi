#define WIN32_MEAN_AND_LEAN
#include <windows.h>

// Compare this to hello-main. "WinMain" don't spin a terminal window when you
// double-click the executable.
//
// Note: usually Windows-compatible compilers determine that behaviour based
// on the exitence of a symbol named WinMain at linking. But this can
// overriden with link flags as well.
int WinMain(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR cmdline, int n_show) {
  MessageBox(NULL, "Text", "Caption", MB_ICONEXCLAMATION);
}
