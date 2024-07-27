#include <SFML/Graphics.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
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

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 650),
                          "SFML Grid Color Change with Toolbox");

  const unsigned int width = 800;
  const unsigned int height = 600;

  // sf::Image nesnesi beyaz olarak başlatılır
  sf::Image image;
  image.create(width, height, sf::Color::White);

  // Bazı pikselleri yeşil ve sarı yapalım
  for (unsigned int y = 0; y < height; ++y) {
    for (unsigned int x = 0; x < width; ++x) {
      if ((x / gridSize) % 2 == 0 && (y / gridSize) % 2 == 0) {
        image.setPixel(x, y, sf::Color::White);
      } else if ((x / gridSize) % 2 == 1 && (y / gridSize) % 2 == 1) {
        image.setPixel(x, y, sf::Color::White);
      }
    }
  }

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
      } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          int x = event.mouseButton.x;
          int y = event.mouseButton.y;

          // Renk kutusunda tıklama
          if (y < toolboxHeight) {
            int colorIndex = x / toolboxHeight;
            if (colorIndex < colors.size()) {
              selectedColor = colors[colorIndex];
            }
          } else {
            // Tıklanan grid hücresinin üst sol köşesini hesaplayalım
            int gridX = (x / gridSize) * gridSize;
            int gridY = ((y - toolboxHeight) / gridSize) *
                        gridSize; // toolboxHeight'ı çıkar

            // Grid hücresini seçilen renkle boyayalım
            for (unsigned int offsetY = 0; offsetY < gridSize; ++offsetY) {
              for (unsigned int offsetX = 0; offsetX < gridSize; ++offsetX) {
                if (gridX + offsetX < width && gridY + offsetY < height) {
                  image.setPixel(gridX + offsetX, gridY + offsetY,
                                 selectedColor);
                }
              }
            }
            flag = true;
            // Texture ve sprite'ı güncelle
            texture.loadFromImage(image);
            sprite.setTexture(texture);
          }
        } else if (event.mouseButton.button == sf::Mouse::Right) {
          int x = event.mouseButton.x;
          int y = event.mouseButton.y;

          // Tıklanan grid hücresinin üst sol köşesini hesaplayalım
          int gridX = (x / gridSize) * gridSize;
          int gridY = ((y - toolboxHeight) / gridSize) *
                      gridSize; // toolboxHeight'ı çıkar

          // Grid hücresini seçilen renkle boyayalım
          for (unsigned int offsetY = 0; offsetY < gridSize; ++offsetY) {
            for (unsigned int offsetX = 0; offsetX < gridSize; ++offsetX) {
              if (gridX + offsetX < width && gridY + offsetY < height) {
                image.setPixel(gridX + offsetX, gridY + offsetY,
                               sf::Color::White);
              }
            }
          }
          flag = true;
          // Texture ve sprite'ı güncelle
          texture.loadFromImage(image);
          sprite.setTexture(texture);
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
      }

      sf::sleep(sf::milliseconds(10));
    }
  }
  return 0;
}
