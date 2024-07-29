#include <SFML/Graphics.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <thread>
#include <vector>

// Grid boyutu
const unsigned int gridSize = 10;

// Renk kutusu boyutu
const unsigned int toolboxHeight = 50;

// Renkler
const std::vector<sf::Color> colors = {sf::Color::Red, sf::Color::Green,
                                       sf::Color::Blue, sf::Color::Yellow,
                                       sf::Color::Black};
sf::Color selectedColor = sf::Color::Black; // Varsayılan renk

void fillPixels(sf::Image &image, sf::Color selectedColor, unsigned int gridX,
                unsigned int gridY, unsigned int gridSizeX,
                unsigned int gridSizeY) {

  for (unsigned int offsetY = 0; offsetY < gridSizeY; ++offsetY) {
    for (unsigned int offsetX = 0; offsetX < gridSizeX; ++offsetX) {

      image.setPixel(gridX + offsetX, gridY + offsetY, selectedColor);
    }
  }
}
int main() {
  sf::RenderWindow window(sf::VideoMode(800, 650),
                          "SFML Grid Color Change with Toolbox");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);
  const unsigned int width = 800;
  const unsigned int height = 600;

  // sf::Image nesnesi beyaz olarak başlatılır
  sf::Image image;
  image.create(width, height, sf::Color::Transparent);

  fillPixels(image, sf::Color::Transparent, 0, 0, width, height);
  // sf::Texture nesnesi sf::Image nesnesinden yüklenir
  sf::Texture texture;
  texture.loadFromImage(image);

  // sf::Sprite nesnesi sf::Texture nesnesi ile oluşturulur
  sf::Sprite sprite;
  sprite.setTexture(texture);
  sprite.setPosition(0, toolboxHeight); // ToolBox'u yukarıda tutmak için
                                        // sprite'ı aşağıya kaydırın

  // Renk seçimi için araç kutusu
  std::vector<sf::RectangleShape> colorBoxes;
  for (size_t i = 0; i < colors.size(); ++i) {
    sf::RectangleShape colorBox(sf::Vector2f(toolboxHeight, toolboxHeight));
    colorBox.setFillColor(colors[i]);
    colorBox.setPosition(i * toolboxHeight, 0);
    colorBoxes.push_back(colorBox);
  }
  bool flag = true;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.key.code == sf::Keyboard::S) {
        // PNG olarak kaydetme
        if (!image.saveToFile("bitmap.png")) {
          std::cerr << "Failed to save image!" << std::endl;
        } else {
          std::cout << "Image saved successfully." << std::endl;
        }
      }

      else if (event.type == sf::Event::MouseButtonPressed) {
        bool deleteFlag = false;
        switch (event.mouseButton.button) {
        case sf::Mouse::Left:
          std::cout << "mere\n";
          flag = true;
          break;
        case sf::Mouse::Right:
          selectedColor = sf::Color::White;
          flag = true;
          break;
        default:
          break;
        }
        if (flag) {

          int x = event.mouseButton.x;
          int y = event.mouseButton.y;

          if (y < toolboxHeight) {
            int colorIndex = x / toolboxHeight;
            if (colorIndex < colors.size()) {
              selectedColor = colors[colorIndex];
            }
          } else {

            int gridX = (x / gridSize) * gridSize;
            int gridY = ((y - toolboxHeight) / gridSize) *
                        gridSize; // toolboxHeight'ı çıkar

            fillPixels(image, selectedColor, gridX, gridY, gridSize, gridSize);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
          }
        }
      }
    }
    if (flag) {
      window.clear(sf::Color::White);

      // ToolBox'u çiz
      for (const auto &colorBox : colorBoxes) {
        window.draw(colorBox);
      }
      std::cout << "HERE\n";
      window.draw(sprite);
      window.display();
      flag = false;
    } else
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // sf::sleep(sf::milliseconds(10));
  }

  return 0;
}
