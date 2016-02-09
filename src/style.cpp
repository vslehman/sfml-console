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

#include "style.hpp"

namespace sfmlConsole {

const Style Style::Default(16, 0.50, 4, '>', '_', sf::Color::White, sf::Color::Blue, sf::Color::White);

//=============================================================================
//  Style::Style()
//-----------------------------------------------------------------------------
Style::Style(size_t fontSize,
             double heightPercentage,
             unsigned int marginSize,
             char promptCharacter,
             char cursorCharacter,
             sf::Color borderColor,
             sf::Color backgroundColor,
             sf::Color fontColor)
  : m_fontSize(fontSize)
  , m_heightPercentage(heightPercentage)
  , m_marginSize(marginSize)
  , m_promptCharacter(promptCharacter)
  , m_cursorCharacter(cursorCharacter)
  , m_borderColor(borderColor)
  , m_backgroundColor(backgroundColor)
  , m_fontColor(fontColor)
{
}


} // namespace sfmlConsole
