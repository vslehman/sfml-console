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

#include "console.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace sfmlConsole {
namespace impl {

const uint32_t Console::ASCII_BEGIN = 0x1F;
const uint32_t Console::ASCII_END   = 0x7D;
const char Console::CURSOR_CHARACTER = '_';

//=============================================================================
//  Console::Console()
//-----------------------------------------------------------------------------
Console::Console()
  : m_isEnabled(false)
  , m_heightPercentage(0.25)
  , m_margin(4)
  , m_visibleLines(10)
  //, minFontSize(16)
  , m_borderColor(sf::Color::Black)
  , m_backgroundColor(sf::Color::Blue)
  //, font(nullptr)
  //, fontSize(16)
  , m_currentInput("")
  , m_cursorMask("")
  , m_tempInput("")
  , m_inputHistoryPosition(-1)
  , m_cursorPosition(0)
{
  //font = &Font::getDefaultFont();

    // Initialize style
  //sf::Vector2u windowSize = window.getSize();
  sf::Vector2u windowSize;

  size_t fontSize = 16;
  m_visibleLines = (windowSize.y*m_heightPercentage)/fontSize;

  //if (fontSize < minFontSize) {
  //  fontSize = minFontSize;
  //}

  m_border = sf::RectangleShape(sf::Vector2f(windowSize.x, windowSize.y*m_heightPercentage));
  m_border.setPosition(0, 0);
  m_border.setFillColor(m_borderColor);

  m_background = sf::RectangleShape(sf::Vector2f(windowSize.x - 2 * m_margin, windowSize.y * m_heightPercentage - 2 * m_margin));
  m_background.setPosition(m_margin, m_margin);
  m_background.setFillColor(m_backgroundColor);

  //sfCurrentInput.setPosition(fontSize / 2 + fontSize, (windowSize.y * m_heightPercentage  - 2 * m_margin) - fontSize);
  //sfCurrentInput.setCharacterSize(fontSize);
  //sfCurrentInput.setFont(*font);

  //sfCursorMask.setPosition(fontSize / 2 + fontSize, (windowSize.y * m_heightPercentage  - 2 * m_margin) - fontSize);
  //sfCursorMask.setCharacterSize(fontSize);
  //sfCursorMask.setFont(*font);

  m_prompt.setPosition(fontSize / 2, (windowSize.y * m_heightPercentage - 2 * m_margin) - fontSize);
  m_prompt.setCharacterSize(fontSize);
  m_prompt.setString(">");
  //m_prompt.setFont(*font);
  m_prompt.setColor(sf::Color::White);

  //sfConsoleHistory.setPosition(fontSize, (windowSize.y * m_heightPercentage  - 2 * m_margin) - fontSize);
  //sfConsoleHistory.setCharacterSize(fontSize);
  //sfConsoleHistory.setFont(*font);
}

//=============================================================================
//  void Console::handleEvent()
//-----------------------------------------------------------------------------
void
Console::handleEvent(const sf::Event& event)
{
  if (!m_isEnabled) {
    return;
  }

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Up) {
      scrollInputUp();
      moveCursorToEnd();
    }
    else if (event.key.code == sf::Keyboard::Down) {
      scrollInputDown();
      moveCursorToEnd();
    }
    else if (event.key.code == sf::Keyboard::Left) {
      moveCursorLeft();
    }
    else if (event.key.code == sf::Keyboard::Right) {
      moveCursorRight();
    }
    else if (event.key.code == sf::Keyboard::PageUp) {
      scrollHistoryUp();
    }
    else if (event.key.code == sf::Keyboard::PageDown) {
      scrollHistoryDown();
    }
    else if (event.key.code == sf::Keyboard::Return) {
      enterInput();
    }
    else if (event.key.code == sf::Keyboard::A) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        moveCursorToBeginning();
      }
    }
    else if (event.key.code == sf::Keyboard::BackSpace) {
      if (m_currentInput.length() > 0 && m_cursorPosition > 0) {
        m_currentInput.erase(m_cursorPosition - 1, 1);
        moveCursorLeft();
      }
    }
  }
  else if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode > ASCII_BEGIN && event.text.unicode < ASCII_END) {
      m_currentInput.insert(m_cursorPosition, 1, static_cast<char>(event.text.unicode));
      moveCursorRight();
    }
  }
}

//=============================================================================
//  void Console::update()
//-----------------------------------------------------------------------------
void Console::update()
{

  //sfCurrentInput.setString(m_currentInput);
  //sfConsoleHistory.setPosition(m_margin, m_margin);

  int startPos = m_outputHistory.size() - m_visibleLines + 1;  // The 1 accounts for the current input

  if (startPos < 0) {
    startPos = 0;
  }

  std::string m_outputHistoryString;

  for (int i = startPos; i < m_outputHistory.size(); i++) {
    m_outputHistoryString += m_outputHistory.at(i) + "\n";
  }

  //sfConsoleHistory.setString(m_outputHistoryString);

  // Calculate cursor offset and apply it to string
  m_cursorMask = "";

  for (int i = 0; i < m_cursorPosition; i++) {
    m_cursorMask += " ";
  }

  m_cursorMask += CURSOR_CHARACTER;

  //sfCursorMask.setString(m_cursorMask);
}

//=============================================================================
//  void Console::show()
//-----------------------------------------------------------------------------
void Console::show()
{
  m_isEnabled = true;
}

//=============================================================================
//  void Console::hide()
//-----------------------------------------------------------------------------
void Console::hide()
{
  m_isEnabled = false;
}

//=============================================================================
//  void Console::toggle()
//-----------------------------------------------------------------------------
void Console::toggle()
{
  m_isEnabled = !m_isEnabled;
}

//=============================================================================
//  void Console::isVisible()
//-----------------------------------------------------------------------------
bool
Console::isVisible() const
{
  return m_isEnabled;
}

//=============================================================================
//  void Console::print()
//-----------------------------------------------------------------------------
void Console::print(const std::string& msg)
{
  m_outputHistory.push_back(msg);
}

//=============================================================================
//  bool Console::registerCommand()
//-----------------------------------------------------------------------------
bool
Console::registerCommand(const std::string& name, const Command& command)
{
  if (findCommand(name) != m_commands.end())
  {
    print("Cannot register \"" + name + "\", a command is already registered with that name.");
    return false;
  }

  m_commands[name] = command;
  print("Registered console command \"" + name + "\"");

  return true;
}

//=============================================================================
//  bool Console::unregisterCommand()
//-----------------------------------------------------------------------------
bool
Console::unregisterCommand(const std::string& name)
{
  CommandMap::const_iterator it = findCommand(name);

  if (it == m_commands.end())
  {
    print("Cannot unregister \"" + name + "\", a command with that name does not exist.");
    return false;
  }

  m_commands.erase(it);
  print("Unregistered console command \"" + name + "\"");

  return true;
}

//=============================================================================
//  bool Console::isCommand()
//-----------------------------------------------------------------------------
bool
Console::isCommand(const std::string& name) const
{
  return findCommand(name) != m_commands.end();
}

//=============================================================================
//  const_iterator Console::findCommand()
//-----------------------------------------------------------------------------
Console::CommandMap::const_iterator
Console::findCommand(const std::string& name) const
{
  return m_commands.find(name);
}

//=============================================================================
//  void Console::draw()
//-----------------------------------------------------------------------------
void
Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(m_border);
  target.draw(m_background);
  //target.draw(indicator);
  //target.draw(sfConsoleHistory);
  //target.draw(sfCurrentInput);
  //target.draw(sfCursorMask);
}

//=============================================================================
//  void Console::insert()
//-----------------------------------------------------------------------------
void Console::insert(std::string text)
{
  m_currentInput.append(text);
  moveCursorToEnd();
}

//=============================================================================
//  void Console::scrollHistoryUp()
//-----------------------------------------------------------------------------
void Console::scrollHistoryUp()
{

}

//=============================================================================
//  void Console::scrollHistoryDown()
//-----------------------------------------------------------------------------
void Console::scrollHistoryDown()
{

}

//=============================================================================
//  void Console::scrollInputUp()
//-----------------------------------------------------------------------------
void Console::scrollInputUp()
{
  if (m_inputHistory.size() == 0) {
    return;
  }

  if (m_inputHistoryPosition == -1) {
    m_tempInput = m_currentInput;
    m_inputHistoryPosition = (int)m_inputHistory.size() - 1;
  }
  else {
    m_inputHistoryPosition--;
  }

  if (m_inputHistoryPosition < 0) {
    m_inputHistoryPosition = 0;
  }

  m_currentInput = m_inputHistory.at(m_inputHistoryPosition);
}

//=============================================================================
//  void Console::scrollInputDown()
//-----------------------------------------------------------------------------
void Console::scrollInputDown()
{
  if (m_inputHistory.size() == 0 || m_inputHistoryPosition == -1) {
    return;
  }

  // If position is bottom of queue
  if (m_inputHistoryPosition == m_inputHistory.size() - 1) {
    m_currentInput = m_tempInput;
    m_inputHistoryPosition = -1;
    return;
  }
  else {
    m_inputHistoryPosition++;
    m_currentInput = m_inputHistory.at(m_inputHistoryPosition);
  }
}

//=============================================================================
//  void Console::moveCursorLeft()
//-----------------------------------------------------------------------------
void Console::moveCursorLeft()
{
  m_cursorPosition--;

  if (m_cursorPosition < 0) {
    moveCursorToBeginning();
  }
}

//=============================================================================
//  void Console::moveCursorRight()
//-----------------------------------------------------------------------------
void Console::moveCursorRight()
{
  m_cursorPosition++;

  if (m_cursorPosition > m_currentInput.size()) {
    moveCursorToEnd();
  }
}

//=============================================================================
//  void Console::moveCursorToBeginning();
//-----------------------------------------------------------------------------
void Console::moveCursorToBeginning()
{
  m_cursorPosition = 0;
}

//=============================================================================
//  void Console::moveCursorToEnd();
//-----------------------------------------------------------------------------
void Console::moveCursorToEnd()
{
  m_cursorPosition = m_currentInput.size();
}

//=============================================================================
//  void Console::clearHistory()
//-----------------------------------------------------------------------------
void Console::clearHistory()
{
  m_outputHistory.clear();
}

//=============================================================================
//  void Console::enterInput()
//-----------------------------------------------------------------------------
void
Console::enterInput()
{

}

} // namespace impl
} // namespace sfmlConsole
