// (c) 2020 Nikolay Neupokoev
// This code is licensed under MIT license (see LICENSE for details)

#ifndef BEAN_WINDOW
#define BEAN_WINDOW

#include <iostream>
#include <functional>
#include <X11/Xlib.h>

#include "BeanFrameBuffer.h"

namespace {

const int window_width = 600;
const int window_height = 600;
const int window_x = 0;
const int window_y = 0;

} //namespace

class BeanWindow {
public:
  BeanWindow() {}
  ~BeanWindow() {}

  void create(int width = window_width,
      int height = window_height,
      int pos_x = window_x,
      int pos_y = window_y) {
    mr_display = XOpenDisplay(NULL);
    if (!mr_display) {
      std::cerr << "No X display" << "\n";
      return;
    }
    mr_width = width;
    mr_height = height;
    mr_screen = DefaultScreen(mr_display);
    mr_parent = DefaultRootWindow(mr_display);
    BeanFrameBuffer bean_frame_buffer = BeanFrameBuffer(mr_display);
    const XVisualInfo* glx_visualinfo = bean_frame_buffer.visual();
    int black = BlackPixel(mr_display, mr_screen);
    const unsigned int border_width = 0;
    const int depth = glx_visualinfo->depth;
    const unsigned int window_class = InputOutput;
    const auto visual = glx_visualinfo->visual;
    const unsigned long attributes_mask = CWBackPixel | CWBorderPixel | CWColormap;
    XSetWindowAttributes attributes;
    attributes.background_pixel = black;
    attributes.border_pixel = black;
    attributes.colormap = XCreateColormap(mr_display, mr_parent, visual, AllocNone);

    mr_window = XCreateWindow(
      mr_display,
      mr_parent,
      pos_x, pos_y,
      width, height,
      border_width,
      depth,
      window_class,
      visual,
      attributes_mask,
      &attributes
    );
    XSelectInput(mr_display, mr_window, StructureNotifyMask|ExposureMask|ButtonPressMask|KeyPressMask);
    XMapWindow(mr_display, mr_window);
  }

  void close() {
    XDestroyWindow(mr_display, mr_window);
    XCloseDisplay(mr_display);
  }

  void setup(std::function<void()> init) {
    init();
  }

  void start_event_loop(std::function<void()> draw) {
    XEvent event;
    bool quit = false;
    while (!quit) {
      if (XPending(mr_display)) {
        XNextEvent(mr_display, &event);
        switch(event.type) {
        case ClientMessage:
        case KeyPress:
          quit = true;
          break;
        }
      } else {
        draw();
      }
    }
    std::cout << "Stop drawing" << "\n";
  }

  int width() const { return mr_width; }
  int height() const { return mr_height; }
  Display* display() const { return mr_display; }
  Window window() const { return mr_window; }
  int screen() const { return mr_screen; }

private:
  Display* mr_display;
  int mr_screen;
  Window mr_parent;
  Window mr_window;
  int mr_width;
  int mr_height;
};

#endif /* end of include guard: BEAN_WINDOW */
