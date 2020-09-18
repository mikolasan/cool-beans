// (c) 2020 Nikolay Neupokoev
// This code is licensed under MIT license (see LICENSE for details)
 
#ifndef BEAN_CONTEXT
#define BEAN_CONTEXT

#include <GL/glx.h>

#include "BeanFrameBuffer.h"

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);


class BeanContext {

public:
  BeanContext(Display* display, GLXContext share_context = nullptr) :
    mr_display(display)
  {
    int glx_major = 0;
    int glx_minor = 0;
    if (!glXQueryVersion(display, &glx_major, &glx_minor) ||
        ((glx_major == 1) && (glx_minor < 3)) ||
        (glx_major < 1)) {
      std::cerr << "Invalid GLX version " << glx_major << "." << glx_minor << "\n";
    } else {
      std::cout << "GLX version " << glx_major << "." << glx_minor << "\n";
    }

    auto m_contextHandle = glXGetCurrentContext();
    if (m_contextHandle == nullptr) {
      std::cerr << "glXGetCurrentContext returned nullptr" << "\n";
    }
    auto m_drawable = glXGetCurrentDrawable();
    if (m_drawable == 0) {
      std::cerr << "glXGetCurrentDrawable returned 0" << "\n";
    }

    BeanFrameBuffer bean_frame_buffer = BeanFrameBuffer(display);
    auto best_fb_config = bean_frame_buffer.fb_config();

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = \
      (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

    const int context_attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
      GLX_CONTEXT_MINOR_VERSION_ARB, 0,
      //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
      None
    };

    mr_glx_context = glXCreateContextAttribsARB(
        display,
        best_fb_config,
        share_context,
        GL_TRUE,
        context_attribs
    );

    // mr_glx_context = glXCreateNewContext(
    //   display,
    //   best_fb_config,
    //   GLX_RGBA_TYPE,
    //   share_context,
    //   GL_TRUE
    // );

    if(!mr_glx_context)
    {
      std::cerr << "Failed to create a glx context for decoding" << std::endl;
      return;
    }
    XSync(display, False);
    std::cout << "The decoding glx context created" << std::endl;

    if (glXIsDirect(display, mr_glx_context)) {
      std::cout << "Direct GLX rendering context obtained" << "\n";
    } else {
      std::cout << "Indirect GLX rendering context obtained" << "\n";
    }

    // const int attrib_list[] = {
    //   GLX_PBUFFER_WIDTH, 600,
    //   GLX_PBUFFER_WIDTH, 600,
    //   GLX_LARGEST_PBUFFER, False,
    //   None
    // };
    // mr_glx_pbuffer = glXCreatePbuffer(display, best_fb_config, attrib_list);

    // // check if pbuffer is supported
    // if (glXMakeContextCurrent(display, m_glx_pbuffer, m_glx_pbuffer, mr_glx_context))
    // {
    //     glXMakeContextCurrent(display, None, None, nullptr);
    //     m_glx_drawable = m_glx_pbuffer;
    // }
    // else
    // {
    //     std::cerr << "glXMakeContextCurrent failed" << std::endl;
    // }

  }

  ~BeanContext() {
    destroy();
  }

  void destroy() {
    if (mr_glx_context) {
      glXMakeCurrent(mr_display, 0, 0);
      glXDestroyContext(mr_display, mr_glx_context);
      mr_glx_context = nullptr;
    }
  }

  void setup_space(int width, int height) const {
    glViewport(0, 0, width, height);       /* Establish viewing area to cover entire window. */
    glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
    glLoadIdentity();             /* Reset project matrix. */
    glOrtho(0, width, 0, height, -1, 1);   /* Map abstract coords directly to window coords. */
    glScalef(1, -1, 1);           /* Invert Y axis so increasing Y goes down. */
    glTranslatef(0, -height, 0);       /* Shift origin up to upper-left corner. */
  }

  GLXContext context() const { return mr_glx_context; }
  GLXPbuffer rendering_area() const { return mr_glx_pbuffer; }

private:
  Display* mr_display;
  GLXContext mr_glx_context{};
  GLXPbuffer mr_glx_pbuffer{};
  GLXDrawable mr_glx_drawable{};
};

#endif /* end of include guard: BEAN_CONTEXT */
