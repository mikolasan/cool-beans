// (c) 2020 Nikolay Neupokoev
// This code is licensed under MIT license (see LICENSE for details)

#ifndef BEAN_FRAME_BUFFER
#define BEAN_FRAME_BUFFER

#include <iostream>
#include <GL/glx.h>

namespace {

const int attrib_list[] = {
  // GLX_RENDER_TYPE, GLX_RGBA_BIT,
  // GLX_RED_SIZE, 8,
  // GLX_GREEN_SIZE, 8,
  // GLX_BLUE_SIZE, 8,
  // GLX_ALPHA_SIZE, 8,
  // GLX_DRAWABLE_TYPE, GLX_PIXMAP_BIT,
  // GLX_DEPTH_SIZE, 0, // frame buffer configurations with no depth buffer are preferred
  // None
  GLX_X_RENDERABLE    , True,
  GLX_DRAWABLE_TYPE   , GLX_PIXMAP_BIT,
  GLX_RENDER_TYPE     , GLX_RGBA_BIT,
  GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
  GLX_RED_SIZE        , 8,
  GLX_GREEN_SIZE      , 8,
  GLX_BLUE_SIZE       , 8,
  GLX_ALPHA_SIZE      , 8,
  GLX_DEPTH_SIZE      , 24,
  GLX_STENCIL_SIZE    , 8,
  GLX_DOUBLEBUFFER    , True,
  //GLX_SAMPLE_BUFFERS  , 1,
  //GLX_SAMPLES         , 4,
  None
};

} // namespace

class BeanFrameBuffer {
public:
  BeanFrameBuffer(Display* display) :
    mr_display(display)
  {
    auto screen = DefaultScreen(display);
    int fb_config_count;
    auto fb_config = glXChooseFBConfig(
      display,
      screen,
      attrib_list,
      &fb_config_count
    );
    if(!fb_config && fb_config_count <= 0)
    {
      std::cerr << "No FB config found" << std::endl;
      return;
    };

    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
    for (int i = 0; i < fb_config_count; ++i) {
      XVisualInfo *vi = glXGetVisualFromFBConfig(display, fb_config[i]);
      if (vi) {
        int samp_buf, samples;
        glXGetFBConfigAttrib(display, fb_config[i], GLX_SAMPLE_BUFFERS, &samp_buf);
        glXGetFBConfigAttrib(display, fb_config[i], GLX_SAMPLES, &samples);
        std::cout << "  Matching fbconfig " << i
          << ", visual ID " << vi->visualid
          << ": SAMPLE_BUFFERS = " << samp_buf
          << ", SAMPLES = " << samples
          << std::endl;
        if (best_fbc < 0 || (samp_buf && samples > best_num_samp))
          best_fbc = i, best_num_samp = samples;
        if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
          worst_fbc = i, worst_num_samp = samples;
      } else {
        std::cerr << "No vi for " << i << "\n";
      }
      XFree(vi);
    }
    std::cout << "Best FB config is " << best_fbc << "\n";
    mr_fb_config = fb_config[best_fbc];
    XFree(fb_config);
  }

  ~BeanFrameBuffer() {}

  GLXFBConfig fb_config() const { return mr_fb_config; }
  XVisualInfo* visual() { return glXGetVisualFromFBConfig(mr_display, mr_fb_config); }

private:
  Display* mr_display;
  GLXFBConfig mr_fb_config;
};

#endif /* end of include guard: BEAN_FRAME_BUFFER */
