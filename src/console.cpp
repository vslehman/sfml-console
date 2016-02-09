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

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <regex>

namespace sfmlConsole {
namespace impl {

const uint32_t Console::ASCII_BEGIN = 0x1F;
const uint32_t Console::ASCII_END   = 0x7D;
const char Console::CURSOR_CHARACTER = '_';

const int INPUT_HISTORY_NO_POSITION = -1;

//=============================================================================
//  Console::Console()
//-----------------------------------------------------------------------------
Console::Console(const sf::RenderWindow& window, const sf::Font& font, Style style)
  : m_font(font)
  , m_style(style)
  , m_isEnabled(false)
  , m_slideSpeed(5)
  , m_visibleLines(10)
  , m_currentInput("")
  , m_cursorMask("")
  , m_tempInput("")
  , m_inputHistoryPosition(INPUT_HISTORY_NO_POSITION)
  , m_cursorPosition(0)
{
  // Initialize style
  m_border.setFillColor(m_style.getBorderColor());
  m_background.setFillColor(m_style.getBackgroundColor());

  m_sfCurrentInput.setFont(font);
  m_sfCursorMask.setFont(font);

  m_prompt.setString(m_style.getPromptCharacter());
  m_prompt.setFont(font);
  m_prompt.setColor(m_style.getFontColor());

  // Initialize console size
  onWindowResize(window.getSize());
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
  else if (event.type == sf::Event::Resized) {
    onWindowResize(sf::Vector2u(event.size.width, event.size.height));
  }
}

//=============================================================================
//  void Console::update()
//-----------------------------------------------------------------------------
void Console::update()
{

  m_sfCurrentInput.setString(m_currentInput);

  // Calculate cursor offset and apply it to string
  m_cursorMask = "";

  for (int i = 0; i < m_cursorPosition; i++) {
    m_cursorMask += " ";
  }

  m_cursorMask += CURSOR_CHARACTER;

  m_sfCursorMask.setString(m_cursorMask);

  // Move console in or out of window
  switch (m_state) {
    case State::OPENING: {
      slideOpen();
      break;
    }
    case State::CLOSING: {
      slideClosed();
      break;
    }
    default: {
      break;
    }
  }
}

//=============================================================================
//  void Console::show()
//-----------------------------------------------------------------------------
void Console::show()
{
  m_isEnabled = true;
  m_state = State::OPENING;
}

//=============================================================================
//  void Console::hide()
//-----------------------------------------------------------------------------
void Console::hide()
{
  m_isEnabled = false;
  m_state = State::CLOSING;
}

//=============================================================================
//  void Console::toggle()
//-----------------------------------------------------------------------------
void Console::toggle()
{
  m_isEnabled = !m_isEnabled;

  if (m_state == State::CLOSED || m_state == State::CLOSING) {
    m_state = State::OPENING;
  }
  else {
    m_state = State::CLOSING;
  }
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
  sf::Text text;
  text.setString(msg);
  text.setFont(m_font);
  text.setColor(sf::Color::White);

  m_outputHistory.push_back(text);
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
  sf::View view(sf::FloatRect(sf::Vector2f(0, 0),
                sf::Vector2f(target.getSize().x, target.getSize().y)));
  target.setView(view);

  target.draw(m_border);
  target.draw(m_background);
  target.draw(m_prompt);

  sf::Vector2f borderPos = m_border.getPosition();
  sf::Vector2f pos = sf::Vector2f(borderPos.x + 2 * m_style.getMarginSize(), borderPos.y + 2 * m_style.getMarginSize());

  int startPos = m_outputHistory.size() - m_visibleLines + 1;  // The 1 accounts for the current input

  if (startPos < 0) {
    startPos = 0;
  }

  int nLines = 0;
  for (int i = startPos; i < m_outputHistory.size() && nLines < m_visibleLines; ++i, ++nLines) {
    sf::Text text = m_outputHistory.at(i);
    text.setPosition(pos.x, pos.y);
    text.setCharacterSize(m_style.getFontSize());
    target.draw(text);

    pos.y += m_style.getFontSize();
  }

  target.draw(m_sfCurrentInput);
  target.draw(m_sfCursorMask);

  target.setView(target.getDefaultView());
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

  if (m_inputHistoryPosition == INPUT_HISTORY_NO_POSITION) {
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
  if (m_inputHistory.size() == 0 || m_inputHistoryPosition == INPUT_HISTORY_NO_POSITION) {
    return;
  }

  // If position is bottom of queue
  if (m_inputHistoryPosition == m_inputHistory.size() - 1) {
    m_currentInput = m_tempInput;
    m_inputHistoryPosition = INPUT_HISTORY_NO_POSITION;
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
  if (m_cursorPosition > 0) {
    m_cursorPosition--;
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
//  Tokens tokenize()
//-----------------------------------------------------------------------------
ConsoleApi::CommandParameters
tokenize(const std::string& input)
{
  ConsoleApi::CommandParameters tokens;

  std::regex re("(\".*\")|(\'.*\')|([^\\s]?[^\\s]+)");

  std::copy(std::sregex_token_iterator(input.begin(), input.end(), re),
            std::sregex_token_iterator(),
            std::back_inserter(tokens));

  return tokens;
}

//=============================================================================
//  void Console::enterInput()
//-----------------------------------------------------------------------------
void
Console::enterInput()
{
  print(m_currentInput);

  // Now that input has been entered, add it to history
  m_inputHistory.push_back(m_currentInput);

  // Reset the history position to scroll to newest input
  m_inputHistoryPosition = INPUT_HISTORY_NO_POSITION;

  // Tokenize input
  ConsoleApi::CommandParameters params = tokenize(m_currentInput);

  // Is there any input?
  if (params.size() == 0) {
    return;
  }

  // Reset prompt
  m_currentInput.clear();
  moveCursorToBeginning();

  // Try to find a command that matches the first parameter
  const std::string& cmd = params.front();
  CommandMap::const_iterator it = findCommand(cmd);

  if (it != m_commands.end()) {
    // Remove command name from parameter list
    params.erase(params.begin());

    // Execute command
    it->second(params);
  }
  else {
    print("Unknown command \"" + cmd + "\"");
  }
}

//=============================================================================
//  void Console::slideClosed()
//-----------------------------------------------------------------------------
void
Console::slideClosed()
{
  m_border.move(0, -m_slideSpeed);
  m_background.move(0, -m_slideSpeed);
  m_sfCurrentInput.move(0, -m_slideSpeed);
  m_sfCursorMask.move(0, -m_slideSpeed);
  m_prompt.move(0, -m_slideSpeed);

  // Is console fully closed?
  if (m_border.getPosition().y <= -m_border.getSize().y) {
    setClosed();
  }
}

//=============================================================================
//  void Console::slideOpen()
//-----------------------------------------------------------------------------
void
Console::slideOpen()
{
  m_border.move(0, m_slideSpeed);
  m_background.move(0, m_slideSpeed);
  m_sfCurrentInput.move(0, m_slideSpeed);
  m_sfCursorMask.move(0, m_slideSpeed);
  m_prompt.move(0, m_slideSpeed);

  // Is console fully open?
  if (m_border.getPosition().y >= 0) {
    setOpen();
  }
}

//=============================================================================
//  void Console::setClosed()
//-----------------------------------------------------------------------------
void
Console::setClosed()
{
  //int camHeight = getCvarValue("window_height");

  //conBack.setPosition(0, -conBack.getSize().y);
  //conFore.setPosition(padding, -conBack.getSize().y + padding);
  //m_sfCurrentInput.setPosition(fontSize/2 + fontSize, -conBack.getSize().y + (conFore.getSize().y  - 2*padding) - fontSize);
  //m_sfCursorMask.setPosition(fontSize/2 + fontSize, -conBack.getSize().y + (conFore.getSize().y  - 2*padding) - fontSize);
  //indicator.setPosition(fontSize/2, -conBack.getSize().y + (conFore.getSize().y - 2*padding) - fontSize);

  m_state = State::CLOSED;
}

//=============================================================================
//  void Console::setOpen()
//-----------------------------------------------------------------------------
void
Console::setOpen()
{
  //conBack.setPosition(0, 0);
  //conFore.setPosition(padding, padding);
  //m_sfCurrentInput.setPosition(fontSize/2 + fontSize, (conFore.getSize().y - 2*padding) - fontSize);
  //m_sfCursorMask.setPosition(fontSize/2 + fontSize, (conFore.getSize().y - 2*padding) - fontSize);
  //indicator.setPosition(fontSize/2, (conFore.getSize().y - 2*padding) - fontSize);

  m_state = State::OPEN;
}

//=============================================================================
//  void Console::onWindowResize()
//-----------------------------------------------------------------------------
void
Console::onWindowResize(const sf::Vector2u& windowSize)
{
  m_border.setSize(sf::Vector2f(windowSize.x, windowSize.y * m_style.getHeightPercentage()));
  m_border.setPosition(0, 0);

  m_background.setSize(sf::Vector2f(windowSize.x - 2 * m_style.getMarginSize(),
                                    windowSize.y * m_style.getHeightPercentage() - 2 * m_style.getMarginSize()));
  m_background.setPosition(m_style.getMarginSize(), m_style.getMarginSize());

  m_sfCurrentInput.setPosition(m_style.getFontSize() / 2 + m_style.getFontSize(),
                               (windowSize.y * m_style.getHeightPercentage()  - 2 * m_style.getMarginSize()) - m_style.getFontSize());
  m_sfCurrentInput.setCharacterSize(m_style.getFontSize());

  m_sfCursorMask.setPosition(m_style.getFontSize() / 2 + m_style.getFontSize(),
                             (windowSize.y * m_style.getHeightPercentage()  - 2 * m_style.getMarginSize()) - m_style.getFontSize());
  m_sfCursorMask.setCharacterSize(m_style.getFontSize());

  m_prompt.setPosition(m_style.getFontSize() / 2,
                       (windowSize.y * m_style.getHeightPercentage() - 2 * m_style.getMarginSize()) - m_style.getFontSize());
  m_prompt.setCharacterSize(m_style.getFontSize());

  m_visibleLines = (m_background.getSize().y - m_style.getFontSize()) / m_style.getFontSize();
}

} // namespace impl
} // namespace sfmlConsole
