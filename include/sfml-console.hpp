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

#ifndef SFML_CONSOLE_HPP
#define SFML_CONSOLE_HPP

#include <functional>
#include <string>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>

namespace sf {
  class Event;
  class RenderWindow;
}

namespace sfmlConsole {

class ConsoleApi : public sf::Drawable
{
public:
  virtual
  ~ConsoleApi(){};

  virtual void
  handleEvent(const sf::Event& event) = 0;

  virtual void
  update() = 0;

public:
  virtual void
  show() = 0;

  virtual void
  hide() = 0;

  virtual void
  toggle() = 0;

  virtual bool
  isVisible() const = 0;

  virtual void
  print(const std::string& msg) = 0;

  virtual void
  clearHistory() = 0;

public:
  typedef std::vector<std::string> CommandParameters;
  typedef std::function<void(const CommandParameters&)> Command;

  virtual bool
  registerCommand(const std::string& name, const Command& command) = 0;

  virtual bool
  unregisterCommand(const std::string& name) = 0;

  virtual bool
  isCommand(const std::string& name) const = 0;

  virtual void
  draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class SfmlConsole : public ConsoleApi
{
public:
  SfmlConsole();

  virtual
  ~SfmlConsole(){};

  virtual void
  handleEvent(const sf::Event& event) override;

  virtual void
  update() override;

public:
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

public:
  typedef std::vector<std::string> CommandParameters;
  typedef std::function<void(const CommandParameters&)> Command;

  virtual bool
  registerCommand(const std::string& name, const Command& command) override;

  virtual bool
  unregisterCommand(const std::string& name) override;

  virtual bool
  isCommand(const std::string& name) const override;

  virtual void
  draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  std::unique_ptr<ConsoleApi> m_impl;
};

} // namespace sfmlConsole

#endif // SFML_CONSOLE_HPP
