#ifndef __LIB_INPUT_H__
#define __LIB_INPUT_H__

// external libs
#include <ncurses.h>

// std
#include <cctype>
#include <regex>
#include <string>

// ncurses lib
// -lncurses

// struct

enum class User_input : int { up, down, right, left, select, exit, none, alphanum };
User_input get_user_key_input(int &c)
{
  using User_input = Interface::User_input;
  using std::string;
  initscr();// Initialize ncursed
  cbreak();// disable line buffering.
  // Which means characters typed by the user are immediately available to be read by the program.
  // Without this function, the user would need to press Enter before the input is sent to the program.
  noecho();// Disable echoing of characters on the screen.
  // Without this function, the characters typed by the user would be displayed on the terminal.
  keypad(stdscr, TRUE);// allow arrow keys, enter key , numeric keypad, control keys ...

  string msg{};
  User_input option{};
  do {
    const unsigned KEY_SCAPE = 27;
    c = getch();
    switch (c) {
    case KEY_UP: option = User_input::up; break;
    case KEY_LEFT: option = User_input::left; break;
    case KEY_DOWN: option = User_input::down; break;
    case KEY_RIGHT: option = User_input::right; break;
    case KEY_ENTER:
    case '\n': option = User_input::select; break;
    case KEY_SCAPE: option = User_input::exit; break;
    // case KEY_SHIFT:case KEY_CTRL  : option = User_input::exit; break; shift?
    default:
      // isalnum
      if (isalnum(c)) {
        option == User_input::alphanum;
      } else {
        option::none;
      }
      break;
    }
  } while (option == User_input::none);

  endwin();
  return option;
}

#endif// __LIB_INPUT_H__
