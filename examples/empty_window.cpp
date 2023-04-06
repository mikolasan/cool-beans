#include <BeanWindow.h>

int main(int argc, char const *argv[])
{
  BeanWindow* window = new BeanWindow();
  window->create(640, 480);
  // window->setup([](){});
  window->start_event_loop([](){});
  window->close();
  delete window;
  return 0;
}
