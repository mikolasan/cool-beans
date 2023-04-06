#include <iostream>
#include <X11/Xlib.h>

int main(int argc, char const *argv[]) 
{
  Display *mr_display = XOpenDisplay(NULL);
  if (!mr_display) {
    std::cerr << "No X display" << "\n";
    return 0;
  }
  std::cerr << "Found X display" << "\n";
  XCloseDisplay(mr_display);
  return 0;
}
