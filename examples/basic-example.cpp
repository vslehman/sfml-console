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

#include "../include/sfml-console.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <exception>

int
main(int argc, char* arv[])
{
  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(640, 480), "SFML-Console");
  sf::Event event;
  sf::Font font;

  // Create SFML Console
  using sfmlConsole::SfmlConsole;
  SfmlConsole console;

  // Register command "hello" which prints "world!" in the console
  console.registerCommand(
    "hello",
    [&console] (const SfmlConsole::CommandParameters& params) {
      console.print("world!");
    }
  );

  // Register command "set" which updates the value of variable using the
  // first parameter
  int variable = 0;
  console.registerCommand(
    "set",
    [&console, &variable] (const SfmlConsole::CommandParameters& params) {
      if (params.size() != 1) {
        console.print("Expected 1 parameter; got " + std::to_string(params.size()));
        return;
      }

      // Try to assign the first parameter to variable. If an error occurs,
      // print the error
      try {
        variable = std::stoi(params.front());
      }
      catch (const std::exception& e) {
        console.print("Error: " + std::string(e.what()));
      }
    }
  );

  // Make the console visible when the program starts
  console.show();

  while (window.isOpen()) {
    // Process SFML events
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
        else if (event.key.code == sf::Keyboard::Space) {
          // Open or close the console
          console.toggle();
        }
      }

      // Pass SFML events to the console
      console.handleEvent(event);
    }

    // Allow the console to update its text
    console.update();

    window.clear(sf::Color::Black);

    // Draw variable's value
    sf::Text text("Variable: " + std::to_string(variable), font);
    text.setColor(sf::Color::White);

    window.draw(text);

    // Draw the console in the foreground of the window
    window.draw(console);

    window.display();
  }

  return 0;
}