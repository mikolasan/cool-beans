#include <iostream>
#include <xcb/xcb.h>
// #include <X11/Xlib.h>

int main(int argc, char const *argv[]) 
{
  // Display *mr_display = XOpenDisplay(NULL);
  // if (!mr_display) {
  //   std::cerr << "No X display" << "\n";
  //   return 0;
  // }
  // std::cerr << "Found X display" << "\n";
  // XCloseDisplay(mr_display);
  // return 0;



	// int n = 0;
	// dpy->xcb = Xcalloc(1, sizeof(_X11XCBPrivate));

	// if(!xcb_parse_display(display, &host, &n, screenp))
	// 	return 0;
	// /* host and n are unused, but xcb_parse_display requires them */
	// free(host);
  
  const char *display = ":0";
  if (argc > 1) {
    display = argv[1];
  }
  std::cout << "connect to display " << display << std::endl;
	xcb_connection_t *c = xcb_connect(display, NULL);
	
	// dpy->fd = -1;
	

	// dpy->xcb->connection = c;
	// dpy->xcb->next_xid = xcb_generate_id(dpy->xcb->connection);

	// dpy->xcb->event_notify = xcondition_malloc();
	// dpy->xcb->reply_notify = xcondition_malloc();
	// if (!dpy->xcb->event_notify || !dpy->xcb->reply_notify)
	// 	return 0;
	// xcondition_init(dpy->xcb->event_notify);
	// xcondition_init(dpy->xcb->reply_notify);
  int error_code = xcb_connection_has_error(c);
  std::cout << "error code: " << error_code << std::endl;
  switch (error_code)
  {
  case XCB_CONN_ERROR:
    std::cout << "Connection failed because of socket errors, pipe errors or other stream errors" << std::endl;
    break;
  case XCB_CONN_CLOSED_EXT_NOTSUPPORTED:
    std::cout << "Connection failed when extension not supported" << std::endl;
    break;
  case XCB_CONN_CLOSED_MEM_INSUFFICIENT:
    std::cout << "Connection failed when memory not available" << std::endl;
    break;
  case XCB_CONN_CLOSED_REQ_LEN_EXCEED:
    std::cout << "Exceeding request length that server accepts" << std::endl;
    break;
  case XCB_CONN_CLOSED_PARSE_ERR:
    std::cout << "Error during parsing display string" << std::endl;
    break;
  case XCB_CONN_CLOSED_INVALID_SCREEN:
    std::cout << "Connection failed because the server does not have a screen matching the display" << std::endl;
    break;
  }

  if (error_code == 0) {
    std::cout << "Connection is good" << std::endl;
  }

  int fd = xcb_get_file_descriptor(c);
  std::cout << "fd: " << fd << std::endl;

  xcb_generic_event_t* event = xcb_poll_for_event(c);
  
  xcb_disconnect(c);
  return 0;
}
