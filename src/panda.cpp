#include "lib.h"



Panda::Panda(int width, int height, char *name)
  : adv(0.0)
{
  _win = bunny_start (width, height, false, name);
  _px  = bunny_new_pixelarray (width, height);
  clear(COLOR(255, 0, 0, 0));

  bunny_set_loop_main_function(__loop);
  bunny_set_key_response(event_handler);
  bunny_set_display_function(__display);
  bunny_loop (_win, 60, this);
}

Panda::Panda(char *name)
  : adv(0.0)
{
const t_bunny_size *s = bunny_get_screen_resolution();
  _win = bunny_start (s->x, s->y, true, name);
  _px  = bunny_new_pixelarray (s->x, s->y);
  clear(COLOR(255, 0, 0, 0));

  bunny_set_loop_main_function(__loop);
  bunny_set_key_response(event_handler);
  bunny_set_display_function(__display);
  bunny_loop (_win, 25, this);
}

Panda::~Panda()
{
  bunny_stop(_win);
}

void Panda::clear(unsigned int color)
{
  for (int i = 0; i < _px->clipable.buffer.width * _px->clipable.buffer.height; ++i)
    set_pixel(i, color);
}

/******************************COLOR******************************/
/******************************************************************/
/******************************COLOR******************************/


double Panda::color_ratio(int      value, 
			       int 	min,
			       int 	max)
{
  return ((double) value - (double) min) / ((double) max - (double) min);
}


unsigned char Panda::color_ratiod(unsigned char	from, 
				      unsigned char	to,
				      double		ratio)
{
  unsigned char result;
  result = (int) ((double) from * ((double) 1 - ratio) + (double) to * ratio);
  return result;
}


unsigned int Panda::mix_color (unsigned int ca, unsigned int cb, double percent)
{
  t_bunny_color a, b, c;
  a.full = ca;
  b.full = cb;
  
  c.argb[RED_CMP] = color_ratiod(a.argb[RED_CMP], b.argb[RED_CMP], percent);
  c.argb[GREEN_CMP] = color_ratiod(a.argb[GREEN_CMP], b.argb[GREEN_CMP], percent);
  c.argb[BLUE_CMP] = color_ratiod(a.argb[BLUE_CMP], b.argb[BLUE_CMP], percent);
  return c.full;
}

int Panda::getWidth() const
{
  return this->_px->clipable.buffer.width;
}

int Panda::getHeight() const
{
  return this->_px->clipable.buffer.height;
}


void Panda::set_xline(t_bunny_position *pos, unsigned int color)
{
  t_bunny_position draw, mi, ma;
  double c = 0.0;
  int vec;
  if (pos[0].x == std::max(pos[0].x, pos[1].x))
    {
      mi = pos[1];
      ma = pos[0];
    }
  else
    {
      mi = pos[0];
      ma = pos[1];
    }
  for (vec = ma.y - mi.y, draw.x = mi.x, draw.y = mi.y + vec * c;
       draw.x <= ma.x;
       c = (double) ((double) draw.x - (double) mi.x) / (double) ((double) ma.x - (double) mi.x),
	 draw.x += 1,
	 draw.y = mi.y + vec * c)
    set_pixel(draw, color);  
}


void Panda::set_yline(t_bunny_position *pos, unsigned int color)
{
  t_bunny_position draw, mi, ma;
  double c = 0.0;
  int vec;
  if (pos[0].y == std::max(pos[0].y, pos[1].y))
    {
      mi = pos[1];
      ma = pos[0];
    }
  else
    {
      mi = pos[0];
      ma = pos[1];
    }
  for (vec = ma.x - mi.x, draw.y = mi.y, draw.x = mi.x + vec * c;
       draw.y <= ma.y;
       c = (double) ((double) draw.y - (double) mi.y) / (double) ((double) ma.y - (double) mi.y),
	 draw.y += 1,
	 draw.x = mi.x + vec * c)
    set_pixel(draw, color);  
}



void Panda::set_eline(t_bunny_position *pos, unsigned int color)
{
  t_bunny_position draw, ma;
  if (pos[0].x == std::max(pos[0].x, pos[1].x))
    {
      ma = pos[0];
      draw = pos[1];
    }
  else
    {
      ma = pos[1];
      draw = pos[0];
    }
  
  for (; draw.x < ma.x; draw.x++)
    {
      set_pixel(draw, color);
      if (draw.y < ma.y)
	draw.y++;
      else
	draw.y--;
      
    }
}


void Panda::set_line(t_bunny_position *pos, unsigned int color)
{
  int x;
  int y;
  x = std::max(pos[0].x, pos[1].x) - std::min(pos[0].x, pos[1].x);
  y = std::max(pos[0].y, pos[1].y) - std::min(pos[0].y, pos[1].y);
  if (x > y)
      set_xline(pos, color);
  else if (x == y)
      set_eline(pos, color);
  else
      set_yline(pos, color);
}


void Panda::set_line(t_bunny_accurate_position pa, t_bunny_accurate_position pb, unsigned int color)
{
  t_bunny_position p[2];
  p[0] = {(int) pa.x, (int) pa.y};
  p[1] = {(int) pb.x, (int) pb.y};
  set_line(p, color);
}

void Panda::set_line(t_bunny_position pa, t_bunny_position pb, unsigned int color)
{
  t_bunny_position p[2];
  p[0] = pa;
  p[1] = pb;
  set_line(p, color);
}




void Panda::set_pixel(int x, int y, unsigned int color)
{
  t_bunny_position pos = {x, y};
  set_pixel(pos, color);
}


void Panda::set_pixel(t_bunny_position pos, unsigned int color)
{
  t_bunny_color old, _new;
  unsigned int *caze = (unsigned int *) _px->pixels;
  old.full = get_pixel(pos);
  _new.full = color;
  if (pos.x <= getWidth() && pos.x >= 0 && pos.y <= getHeight() && pos.y >= 0)
    caze[pos.x + pos.y * _px->clipable.buffer.width] = mix_color(old.full, color, (double) _new.argb[ALPHA_CMP] / 255.0);
}

void Panda::set_pixel(t_bunny_accurate_position pos, unsigned int color)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  t_bunny_color old, _new;
  old.full = get_pixel(pos);
  _new.full = color;
  if ((int) pos.x <= getWidth() && (int) pos.x >= 0 && pos.y <= getHeight() && pos.y >= 0)
    caze[(int) pos.x + (int) pos.y * _px->clipable.buffer.width] = mix_color(old.full, color, (double) _new.argb[ALPHA_CMP] / 255.0);
}

void Panda::set_blood(t_bunny_accurate_position pos, int rec)
{
  unsigned int color = COLOR(255, 255, 0, 0);
  set_pixel (pos, color);
  if (rec > 0)
    {
      t_bunny_accurate_position p = {pos.x + 1, pos.y};
      if (rand()%rec == 1)
	set_blood(p, rec - 1);
      p = {pos.x - 1, pos.y};
      if (rand()%rec == 1)
	set_blood(p, rec - 1);
      p = {pos.x, pos.y + 1};
      if (rand()%rec == 1)
	{
	  set_blood(p, rec - 1);
	  p = {pos.x - 1, pos.y + 1};
	  set_blood(p, rec - 1);
	  p = {pos.x + 1, pos.y + 1};
	  set_blood(p, rec - 1);
	}
    }
}

void Panda::set_blood(t_bunny_accurate_position pos)
{
  int LIM = 10;
  unsigned int color = COLOR(255, 255, 0, 0);
  set_pixel (pos, color);
  t_bunny_accurate_position p = {pos.x + 1, pos.y};
  if (rand()%3 == 1)
    set_blood(p, LIM);
  p = {pos.x - 1, pos.y};
  if (rand()%3 == 1)
    set_blood(p, LIM);
  p = {pos.x, pos.y + 1};
  if (rand()%3 == 1)
    set_blood(p, LIM);
  p = {pos.x, pos.y - 1};
  if (rand()%3 == 1)
    set_blood(p, LIM);
}


void Panda::set_pixel(int i, unsigned int color)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  t_bunny_color old, _new;
  old.full = caze[i];
  _new.full = color;
  caze[i] = mix_color(old.full, color, (double) _new.argb[ALPHA_CMP] / 255.0);
}

unsigned int Panda::get_pixel(t_bunny_position pos)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  return caze[pos.x + pos.y * _px->clipable.buffer.width];
}


unsigned int Panda::get_pixel(t_bunny_accurate_position pos)
{
  unsigned int *caze = (unsigned int *) _px->pixels;
  return caze[(int) pos.x + (int) pos.y * _px->clipable.buffer.width];
}



void Panda::set_square (t_bunny_position *pos, unsigned int color)
{
  t_bunny_position p[2];
  p[0] = pos[0];
  p[1] = {pos[1].x, pos[0].y};
  set_line(p, color);
  p[0] = pos[1];
  p[1] = {pos[0].x, pos[1].y};
  set_line(p, color);
  p[0] = pos[1];
  p[1] = {pos[1].x, pos[0].y};
  set_line(p, color);
  p[0] = pos[0];
  p[1] = {pos[0].x, pos[1].y};
  set_line(p, color);  
}


static t_bunny_accurate_position for_angle (t_bunny_position pos, int radius, double i)
{
  t_bunny_accurate_position draw;
  draw.x = pos.x + radius * cos (i);
  draw.y = pos.y - radius * sin (i);
  return draw;
}

void Panda::set_circle (t_bunny_position pos, int radius, unsigned int color)
{
  t_bunny_accurate_position draw = {(double) pos.x, (double) pos.y};
  for (double i = 0.0; i < M_PI * 2; i += ((M_PI * 2) / (360 * radius)))
    {
      draw = for_angle (pos, radius, i);
      set_pixel(draw, color);
    }
  
}



void Panda::panda_blood_stream (t_bunny_accurate_position pos)
{
  for (double i = 0.0; i <= M_PI; i += M_PI / 20)
      add_particle(pos, cos(i));
}

void Panda::add_particle (t_bunny_accurate_position pos, double x_speed)
{
  t_particule *_p = new t_particule;
  _p->pos = { pos.x, pos.y};
  _p->speed = {x_speed, - (rand()%5 + 1)};
  _particule.push_back(_p);
}

void Panda::run_particle()
{
  int i = 0;
  for (auto it = _particule.begin(); it != _particule.end(); ++i, ++it)
    {
      set_blood(_particule[i]->pos);
      if (_particule[i]->pos.x + _particule[i]->speed.x < (double) getWidth() && _particule[i]->pos.x + _particule[i]->speed.x > 0.0) 
	_particule[i]->pos.x += _particule[i]->speed.x;
      else
	{
	  delete _particule[i];
	  _particule.erase(it);
	}
      _particule[i]->pos.y -= _particule[i]->speed.y;
      if (_particule[i]->pos.y >= getHeight())
	{
	  delete _particule[i];
	  _particule.erase(it);
	}
    }
  _particule.shrink_to_fit();
}
  
void Panda::set_pentacle(int nb, unsigned int color)
{
  t_bunny_position pos = {getWidth() / 2, getHeight() / 2};
  int radius = std::min(getWidth() / 2 * 3.0 / 4.0, getHeight() / 2 * 3.0 / 4.0);
  double angle = (2 * M_PI) / nb;
  adv += 0.007;
  set_circle(pos, radius, color);


  t_bunny_position draw[2];
  for (int i = 0; i < nb; ++i)
    {
      draw[0] = {(int) (pos.x + radius * cos (angle * i + adv)), (int) (pos.y - radius * sin (angle * i + adv))};
      draw[1] = {(int) (pos.x + radius * cos (angle * i + M_PI + adv)), (int) (pos.y - radius * sin (angle * i + M_PI + adv))};
      set_line(draw, color);
      if (angle * i < M_PI / 2)
	draw[1] = {(int) (pos.x + radius * cos (angle * (i + 1) + M_PI + adv)), (int) (pos.y - radius * sin (angle * (i + 1) + M_PI + adv))};
      else if (angle * i > M_PI / 2 && angle * i <= M_PI)
	draw[1] = {(int) (pos.x + radius * cos (angle * (i - 1) + M_PI + adv)), (int) (pos.y - radius * sin (angle * (i - 1) + M_PI + adv))};
      set_line(draw, color);

      if (rand()%100 > 80)
	add_particle (for_angle (pos, radius, angle * i + adv), cos(angle * i + adv));
    }
  run_particle();
}
