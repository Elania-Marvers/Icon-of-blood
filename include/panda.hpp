class Panda
{

private: // set line
  void set_xline(t_bunny_position *pos, unsigned int color);
  void set_yline(t_bunny_position *pos, unsigned int color);
  void set_eline(t_bunny_position *pos, unsigned int color);
  
public:
  t_bunny_window *_win;
  t_bunny_pixelarray *_px;
  
public:
  Panda(char *name);
  Panda(int, int, char *);
  ~Panda(void);

  void clear(unsigned int color);
  int getWidth() const;
  int getHeight() const;

  void set_pixel(int i, unsigned int color);  
  void set_pixel (int, int, unsigned int);
  void set_pixel(t_bunny_position pos, unsigned int color);
  void set_pixel(t_bunny_accurate_position pos, unsigned int color);
  void set_line(t_bunny_position *pos, unsigned int color);
  void set_line(t_bunny_position pa, t_bunny_position pb, unsigned int color);
  void set_line(t_bunny_accurate_position pa, t_bunny_accurate_position pb, unsigned int color);
  
  unsigned char color_ratiod(unsigned char from, unsigned char to, double ratio);
  unsigned int mix_color (unsigned int ca, unsigned int cb, double percent);
  double color_ratio(int value, int min, int max);
  void set_blood(t_bunny_accurate_position pos);
  void set_blood(t_bunny_accurate_position pos, int rec);

  unsigned int get_pixel(t_bunny_position pos);
  unsigned int get_pixel(t_bunny_accurate_position pos);



  void set_square (t_bunny_position *pos, unsigned int color);
  void set_circle (t_bunny_position pos, int radius, unsigned int color);

private:
  double adv;
  std::vector<t_particule *> _particule;
  
public:
  void set_pentacle (int nb, unsigned int color);
  void add_particle (t_bunny_accurate_position pos, double x_speed);
  void run_particle();
  void panda_blood_stream (t_bunny_accurate_position pos);
  
};

t_bunny_response __loop (void *);
t_bunny_response __display(void *);
t_bunny_response event_handler(t_bunny_event_state, t_bunny_keysym, void *);
