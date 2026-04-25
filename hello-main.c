#define WIN32_MEAN_AND_LEAN
#include <windows.h>

// Compare this to hello-winmain. "main" spins a terminal window when you
// invoke it with double-clicking the executable
int main() {
  MessageBox(NULL, "Text", "Caption", MB_ICONEXCLAMATION);
}
