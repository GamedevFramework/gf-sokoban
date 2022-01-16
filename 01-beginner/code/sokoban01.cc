#include <algorithm>
#include <vector>

#include <gf/Array2D.h>
#include <gf/Color.h>
#include <gf/Direction.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/Window.h>

// tag::ns[]
namespace skb {

// end::ns[]

  // tag::data[]
  const char LevelData[] = {
    "  #####"
    "###@ .#"
    "# $ #.#"
    "#  $$ #"
    "#.  # #"
    "#   $.#"
    "#######"
  };

  constexpr gf::Vector2i LevelSize = gf::vec(7, 7);

  // end::data[]

  // tag::hero[]
  struct Hero {
    gf::Vector2i position;
  };

  // end::hero[]


  // tag::make_hero[]
  Hero makeHero(gf::Span<const char> data, gf::Vector2i size) {
    Hero hero = { gf::vec(0, 0) };
    gf::Vector2i position = gf::vec(0, 0);

    for (auto c : data) {
      if (c == '@' || c == '+') {
        hero.position = position;
      }

      ++position.x;

      if (position.x == size.width) {
        position.x = 0;
        ++position.y;
      }
    }

    return hero;
  }

  // end::make_hero[]

  struct Boxes {
    std::vector<gf::Vector2i> positions;

    bool hasBox(gf::Vector2i coordinates) const {
      for (auto position : positions) {
        if (position == coordinates) {
          return true;
        }
      }

      return false;
    }

    void changeBox(gf::Vector2i from, gf::Vector2i to) {
      for (auto & position : positions) {
        if (position == from) {
          position = to;
          return;
        }
      }

      assert(false);
    }
  };

  Boxes makeBoxes(gf::Span<const char> data, gf::Vector2i size) {
    Boxes boxes;
    gf::Vector2i position = gf::vec(0, 0);

    for (auto c : data) {
      if (c == '$' || c == '*') {
        boxes.positions.push_back(position);
      }

      ++position.x;

      if (position.x == size.width) {
        position.x = 0;
        ++position.y;
      }
    }

    return boxes;
  }


  enum class Block {
    Wall,
    Goal,
    Floor,
  };

  struct Level {
    gf::Array2D<Block, int> blocks;

    Level(gf::Vector2i size) : blocks(size) { }
  };

  Level makeLevel(gf::Span<const char> data, gf::Vector2i size) {
    Level level(size);
    gf::Vector2i position = gf::vec(0, 0);

    for (auto c : data) {
      switch (c) {
        case ' ':
        case '@':
        case '$':
          level.blocks(position) = Block::Floor;
          break;
        case '.':
        case '+':
        case '*':
          level.blocks(position) = Block::Goal;
          break;
        case '#':
          level.blocks(position) = Block::Wall;
          break;
        case '\0':
          break;
        default:
          assert(false);
          break;
      }

      ++position.x;

      if (position.x == size.width) {
        position.x = 0;
        ++position.y;
      }
    }

    return level;
  }

// tag::ns[]
}
// end::ns[]


int main() {
  constexpr gf::Vector2i ScreenSize = gf::vec(800, 600);
  gf::Window window("sokoban01", ScreenSize);
  gf::RenderWindow renderer(window);

  skb::Hero hero = skb::makeHero(skb::LevelData, skb::LevelSize);
  skb::Boxes boxes = skb::makeBoxes(skb::LevelData, skb::LevelSize);
  skb::Level level = skb::makeLevel(skb::LevelData, skb::LevelSize);

  gf::Direction direction = gf::Direction::Center;

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.keycode) {
            case gf::Keycode::Up:
              direction = gf::Direction::Up;
              break;
            case gf::Keycode::Right:
              direction = gf::Direction::Right;
              break;
            case gf::Keycode::Down:
              direction = gf::Direction::Down;
              break;
            case gf::Keycode::Left:
              direction = gf::Direction::Left;
              break;
          }
      }
    }

    if (direction != gf::Direction::Center) {
      gf::Vector2i target = hero.position + gf::displacement(direction);

      if (level.blocks(target) != skb::Block::Wall) {
        if (boxes.hasBox(target)) {
          gf::Vector2i behind = target + gf::displacement(direction);

          if (level.blocks(behind) != skb::Block::Wall && !boxes.hasBox(behind)) {
            hero.position = target;
            boxes.changeBox(target, behind);

            if (std::all_of(boxes.positions.begin(), boxes.positions.end(), [&level](gf::Vector2i coordinates) {
              return level.blocks(coordinates) == skb::Block::Goal;
            })) {
              return 0;
            }
          }
        } else {
          hero.position = target;
        }
      }

      direction = gf::Direction::Center;
    }



    renderer.clear();

    constexpr float BlockSize = 64.0f;
    constexpr float BoxSize = BlockSize * 0.8f;
    constexpr float HeroRadius = BlockSize * 0.3f;
    constexpr float HeroThickness = BlockSize * 0.05f;

    gf::Vector2f offset = (ScreenSize - skb::LevelSize * BlockSize) / 2;
    gf::Vector2i coordinates;

    for (coordinates.y = 0; coordinates.y < skb::LevelSize.height; ++coordinates.y) {
      for (coordinates.x = 0; coordinates.x < skb::LevelSize.width; ++coordinates.x) {
        gf::RectangleShape block({ BlockSize, BlockSize });

        if (level.blocks(coordinates) == skb::Block::Wall) {
          block.setColor(gf::Color::Red);
        } else {
          block.setColor(gf::Color::Gray(0.75f));
        }

        block.setPosition(coordinates * BlockSize + offset);
        renderer.draw(block);

        if (level.blocks(coordinates) == skb::Block::Goal) {
          gf::RectangleShape goal({ BlockSize / 5, BlockSize / 5 });
          goal.setColor(gf::Color::Black);
          goal.setPosition(coordinates * BlockSize + offset + BlockSize / 2);
          goal.setAnchor(gf::Anchor::Center);
          renderer.draw(goal);
        }
      }
    }

    for (auto position : boxes.positions) {
      gf::RectangleShape box({ BoxSize, BoxSize });
      box.setColor(gf::Color::darker(gf::Color::Blue));
      box.setPosition(position * BlockSize + offset + BlockSize / 2);
      box.setAnchor(gf::Anchor::Center);
      renderer.draw(box);
    }

    gf::CircleShape pusher(HeroRadius);
    pusher.setColor(gf::Color::Orange);
    pusher.setOutlineColor(gf::Color::darker(gf::Color::Orange));
    pusher.setOutlineThickness(HeroThickness);
    pusher.setPosition(hero.position * BlockSize + offset + BlockSize / 2);
    pusher.setAnchor(gf::Anchor::Center);
    renderer.draw(pusher);

    renderer.display();
  }

}
