#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>

int main(int argc, char ** argv)
{
  const std::vector<std::string> args(argv, argv + argc);

  // We've already tested, so the program is done
  if (args.size() > 1 && args[1] == "--test")
    return 0;

  // Create main window
  sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Graphics");

  sf::RectangleShape rectangle(sf::Vector2f(10.0,10.0));
  rectangle.setPosition(100.0, 200.0);
  rectangle.setFillColor(sf::Color(0, 0, 0));
  window.draw(rectangle);

  // Display all shapes
  window.display();

  sf::Texture texture;
  texture.create(window.getSize().x, window.getSize().y);
  texture.update(window);
  texture.copyToImage().saveToFile("test.png");
}
