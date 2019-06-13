// Screen class definition
// C++ Primer fifth edition
// All hand typed by LCL
// 20190602

#ifndef _SCREEN_H
#define _SCREEN_H

class Screen{
  friend class Window_mgr;
public:
  typedef std::string::size_type pos;
  // Because there is self-defined construcor, the default is a must
  // to reserve the default construcor.
  Screen() = default;
  Screen(pos ht, pos wd): height(ht), width(wd), contents(ht*wd,' ') {}
  Screen(pos ht, pos wd, char c): height(ht), width(wd), contents(ht*wd,c) {}
  char get() const { return contents[cursor];}
  inline char get(pos ht, pos wd) const; // overloaded
  Screen &move(pos r, pos c);
  Screen &set(char);
  Screen &set(pos, pos, char);
  // display non-const edition
  Screen &display(std::ostream &os){do_display(os);return *this;}
  // display const edition, overloaded
  const Screen &display(std::ostream &os) const{do_display(os);return *this;}

private:
  void do_display(std::ostream &os) const {os << contents;}
  pos cursor = 0;
  pos height = 0, width = 0;
  std::string contents;
};

inline
Screen &Screen::move(pos r, pos c){
  pos row = r * width;
  cursor = row + c;
  return *this;
}

inline
char Screen::get(pos ht, pos wd) const{
  pos row = ht * width;
  return contents[row + wd];
}

inline
Screen& Screen::set(char c){
  contents[cursor] = c;
  return *this;
}

inline
Screen& Screen::set(pos r, pos col, char c){
  contents[r*width+col] = c;
  return *this;
}

class Window_mgr{
public:
  typedef std::vector<Screen>::size_type ScreenIndex;
  void clear(ScreenIndex);
  ScreenIndex addScreen(const Screen &);
  void print(ScreenIndex);
private:
  std::vector<Screen> screens;
};

void Window_mgr::clear(ScreenIndex i){
  Screen &s = screens[i];
  s.contents = std::string(s.height*s.width,' ');
}

Window_mgr::ScreenIndex
Window_mgr::addScreen(const Screen &s){
  screens.push_back(s);
  return screens.size() - 1;
}

void Window_mgr::print(ScreenIndex i){
  screens[i].display(std::cout);
}

#endif
