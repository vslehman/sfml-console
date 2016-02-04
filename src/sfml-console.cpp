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

#include "sfml-console.hpp"

#include "impl/console.hpp"

namespace sfmlConsole {

SfmlConsole::SfmlConsole()
  : m_impl(new impl::Console)
{
}

void
SfmlConsole::handleEvent(const sf::Event& event)
{
  m_impl->handleEvent(event);
}

void
SfmlConsole::update()
{
  m_impl->update();
}

void
SfmlConsole::show()
{
  m_impl->show();
}

void
SfmlConsole::hide()
{
  m_impl->hide();
}

void
SfmlConsole::toggle()
{
  m_impl->toggle();
}

bool
SfmlConsole::isVisible() const
{
  m_impl->isVisible();
}

void
SfmlConsole::print(const std::string& msg)
{
  m_impl->print(msg);
}

void
SfmlConsole::clearHistory()
{
  m_impl->clearHistory();
}

bool
SfmlConsole::registerCommand(const std::string& name, const Command& command)
{
  return m_impl->registerCommand(name, command);
}

bool
SfmlConsole::unregisterCommand(const std::string& name)
{
  return m_impl->unregisterCommand(name);
}

bool
SfmlConsole::isCommand(const std::string& name) const
{
  return m_impl->isCommand(name);
}

void
SfmlConsole::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  m_impl->draw(target, states);
}

} // namespace sfmlConsole
