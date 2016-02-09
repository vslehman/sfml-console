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

#ifndef SFML_CONSOLE_STYLE_HPP
#define SFML_CONSOLE_STYLE_HPP

#include <SFML/Graphics/Color.hpp>

#include <cstddef>

namespace sfmlConsole {

class Style
{
public:
  Style(size_t fontSize,
        double heightPercentage,
        unsigned int marginSize,
        char promptCharacter,
        char cursorCharacter,
        sf::Color borderColor,
        sf::Color backgroundColor,
        sf::Color fontColor);
public:
  size_t
  getFontSize() const
  {
    return m_fontSize;
  }

  double
  getHeightPercentage() const
  {
    return m_heightPercentage;
  }

  unsigned int
  getMarginSize() const
  {
    return m_marginSize;
  }

  char
  getPromptCharacter() const
  {
    return m_promptCharacter;
  }

  char
  getCursorCharacter() const
  {
    return m_cursorCharacter;
  }

  sf::Color
  getBorderColor() const
  {
    return m_borderColor;
  }

  sf::Color
  getBackgroundColor() const
  {
    return m_backgroundColor;
  }

  sf::Color
  getFontColor() const
  {
    return m_fontColor;
  }

public:
  void
  setFontSize(size_t size)
  {
    m_fontSize = size;
  }

  void
  setHeightPercentage(double pct)
  {
    m_heightPercentage = pct;
  }

  void
  setMarginSize(unsigned int size)
  {
    m_marginSize = size;
  }

  void
  setPromptCharacter(char promptCharacter)
  {
    m_promptCharacter = promptCharacter;
  }

  void
  setCursorCharacter(char cursorCharacter)
  {
    m_cursorCharacter = cursorCharacter;
  }

  void
  setBorderColor(sf::Color color)
  {
    m_borderColor = color;
  }

  void
  setBackgroundColor(sf::Color color)
  {
    m_backgroundColor = color;
  }

  void
  setFontColor(sf::Color color)
  {
    m_fontColor = color;
  }

public:
  static const Style Default;

private:
  size_t m_fontSize;
  double m_heightPercentage;
  unsigned int m_marginSize;
  char m_promptCharacter;
  char m_cursorCharacter;

  sf::Color m_borderColor;
  sf::Color m_backgroundColor;
  sf::Color m_fontColor;
};

} // namespace sfmlConsole

#endif // SFML_CONSOLE_STYLE_HPP
