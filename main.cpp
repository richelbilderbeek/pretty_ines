#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <vector>

///Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_file_present(const std::string& filename)
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> read_file_as_text(const std::string& filename)
{
  assert(is_file_present(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

/// Returns data like matrix. Remember the columns yourself :-)
std::vector<std::vector<double>> read_data(const std::string& filename)
{
  assert(is_file_present(filename));
  auto text = read_file_as_text(filename);
  //Remove first two lines, that hold the header
  //Remove last line, as it is empty

  text.erase(text.begin());
  text.erase(text.begin());
  text.erase(text.end());

  std::vector<std::vector<double>> data(text.size());
  return data;
}

void test()
{
  const std::string filename{"seed_1610984655324250886_results.csv"};
  //File must be present
  {
    assert(!is_file_present("absnegsefgpew.serguiuer"));
    assert(is_file_present(filename));
  }
  //Data has 19 rows
  {
    const auto text = read_file_as_text(filename);
    assert(text.size() > 19);
  }
  //Data has 19 rows
  {
    const auto d = read_data("seed_1610984655324250886_results.csv");
    assert(d.size() == 19);
  }
  //Data has 37 columns

}

int main(int argc, char ** argv)
{
  test();

  const std::vector<std::string> args(argv, argv + argc);

  // We've already tested, so the program is done
  if (args.size() > 1 && args[1] == "--test")
    return 0;

  // Create main window
  sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Graphics");

  sf::RectangleShape rectangle(sf::Vector2f(10.0,10.0));
  rectangle.setPosition(100.0, 200.0);
  rectangle.setFillColor(sf::Color(255, 128, 0));
  window.draw(rectangle);

  // Display all shapes
  window.display();

  sf::Texture texture;
  texture.create(window.getSize().x, window.getSize().y);
  texture.update(window);
  texture.copyToImage().saveToFile("test.png");
}
