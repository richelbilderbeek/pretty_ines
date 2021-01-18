#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <vector>
#include <boost/lexical_cast.hpp>

//From https://stackoverflow.com/a/37454181
std::vector<std::string> split(const std::string& str, const std::string& delim)
{
  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do
  {
    pos = str.find(delim, prev);
    if (pos == std::string::npos) pos = str.length();
    std::string token = str.substr(prev, pos-prev);
    if (!token.empty()) tokens.push_back(token);
    prev = pos + delim.length();
  }
  while (pos < str.length() && prev < str.length());
  return tokens;
}

std::vector<double> split_to_doubles(const std::string& str, const std::string& delim)
{
  const std::vector<std::string> text = split(str, delim);
  std::vector<double> ds;
  ds.reserve(text.size());
  std::transform(
    std::begin(text),
    std::end(text),
    std::back_inserter(ds),
    [](const std::string& s)
    {
      if (s.empty()) return 0.0;
      if (s == " ") return 0.0;
      return boost::lexical_cast<double>(s);
      //return std::stod(s);
    }
  );
  return ds;
}

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
  const int n_lines = text.size();
  for (int i = 0; i != n_lines; ++i)
  {
    data[i] = split_to_doubles(text[i], ", ");
  }
  return data;
}


void test()
{
  const std::string first_line{"500, 0.5, 0.5, 0.56, 0.44, 0.051882, 0.0656406, 37, 0.52, 0.48, 0.52, 0.48, 0.044718, 0.057978, 40, 0.48, 0.52, 0.6, 0.4, 0.03924, 0.060282, 36, 0.48, 0.52, 0.52, 0.48, 0.055404, 0.059424, 47, 0.5, 0.5, 0.54, 0.46, 0.041742, 0.03954, 32, "};
  // Splitting should split correctly
  {
    assert(split(first_line, ", ")[0] == "500");
    assert(split(first_line, ", ")[1] == "0.5");
    assert(split(first_line, ", ")[2] == "0.5");
  }
  // Splitting to double should split correctly
  {
    const auto ds = split_to_doubles(first_line, ", ");
    assert(ds[0] == 500);
    assert(ds[1] == 0.5);
    assert(ds[2] == 0.5);
  }
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
  //Data has 36 columns (the last one is empty)
  {
    const auto d = read_data("seed_1610984655324250886_results.csv");

    assert(d[0].size() == 36);

  }
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
