#include "lib.h"

t_bunny_response event_handler(t_bunny_event_state state, t_bunny_keysym key, __attribute__((unused))void *data)
{   
  return state == GO_DOWN && key == BKS_ESCAPE ? EXIT_ON_SUCCESS : GO_ON;
}

t_bunny_response __loop (void *data)
{
  (void) data;
  return GO_ON;
}

t_bunny_response __display(void *data)
{
  Panda* d = (Panda *)data;
  d->clear(COLOR(30, 0, 0, 0));
  //  d->clear(COLOR(255, 0, 0, 0));

  if (rand()%100 > 96)
    {
      t_bunny_accurate_position p = {(double) (rand()%(d->getWidth())), (double) (rand()%(d->getHeight()))};
      d->panda_blood_stream (p);
    }

  if (rand()%100 > 96)
    d->set_pentacle(12, COLOR(255, 255, 255, 255));
  else
    d->set_pentacle(12, COLOR(140, 255, 0, 0));

  bunny_blit (&d->_win->buffer, &d->_px->clipable, NULL);
  bunny_display(d->_win);
  return GO_ON;
}

