#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  gf::Window window("sokoban01", { 800, 600 });
  gf::RenderWindow renderer(window);

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;
      }
    }

    renderer.clear();
    renderer.display();
  }

}
