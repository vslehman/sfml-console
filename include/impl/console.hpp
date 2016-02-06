/*
===============================================================================
  SFML-Console: Developer Console Library for SFML
  Copyright (C) 2016  Vince Lehman (vlehman1@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  A copy of the full license can be found in LICENSE.txt.
===============================================================================
*/

#ifndef IMPL_CONSOLE_HPP
#define IMPL_CONSOLE_HPP

#include "../sfml-console.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <map>

namespace sfmlConsole {
namespace impl {

class Console : public ConsoleApi
{
public:
  typedef std::map<const std::string, Command> CommandMap;

  Console(const sf::RenderWindow& window, const sf::Font& font);

  virtual
  ~Console(){};

public:
  virtual void
  handleEvent(const sf::Event& event) override;

  virtual void
  update() override;

  virtual void
  draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  virtual void
  show() override;

  virtual void
  hide() override;

  virtual void
  toggle() override;

  virtual bool
  isVisible() const override;

  virtual void
  print(const std::string& msg) override;

  virtual void
  clearHistory() override;

  virtual bool
  registerCommand(const std::string& name, const Command& command) override;

  virtual bool
  unregisterCommand(const std::string& name) override;

  virtual bool
  isCommand(const std::string& name) const override;

  virtual CommandMap::const_iterator
  findCommand(const std::string& name) const;

public:
  bool
  isEnabled();

  void
  printCommands();

  void
  printCvars();

  void
  printHelp(std::string name);

  void
  insert(std::string text);

protected:
  void
  scrollHistoryUp();

  void
  scrollHistoryDown();

  void
  scrollInputUp();

  void
  scrollInputDown();

  void
  moveCursorLeft();

  void
  moveCursorRight();

  void
  moveCursorToBeginning();

  void
  moveCursorToEnd();

  virtual
  void enterInput();

private:
  void
  setOpen();

  void
  setClosed();

  void
  slideOpen();

  void
  slideClosed();

private:
  bool m_isEnabled;
  size_t m_cursorPosition;
  int m_inputHistoryPosition;
  size_t m_visibleLines;
  double m_heightPercentage;
  int m_slideSpeed;
  unsigned int m_margin;

  std::string m_currentInput;
  std::string m_tempInput;
  std::string m_cursorMask;

  std::vector<std::string> m_outputHistory;
  std::vector<std::string> m_inputHistory;

  sf::RectangleShape m_border;
  sf::RectangleShape m_background;

  sf::Color m_borderColor;
  sf::Color m_backgroundColor;

  sf::Text m_prompt;

  CommandMap m_commands;

private:
  enum class State {
    CLOSED = 0,
    OPEN,
    OPENING,
    CLOSING
  };

  State m_state;

private:
  const sf::Font& m_font;

  sf::Text m_sfCurrentInput;
  sf::Text m_sfConsoleHistory;
  sf::Text m_sfCursorMask;

private:
  static const uint32_t ASCII_BEGIN;
  static const uint32_t ASCII_END;

  static const char CURSOR_CHARACTER;
};

} // namespace impl
} // namespace sfmlConsole

#endif // IMPL_CONSOLE_HPP
