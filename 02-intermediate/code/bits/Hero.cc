#include "Hero.h"

#include <cassert>

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

#include "Boxes.h"
#include "Level.h"

namespace skb {

  Hero::Hero(const gf::TextureAtlas& atlas, const Level& level, Boxes& boxes)
  : gf::Entity(3)
  , m_texture(atlas.getTexture())
  , m_leftRect(atlas.getTextureRect("player_12.png"))
  , m_rightRect(atlas.getTextureRect("player_09.png"))
  , m_downRect(atlas.getTextureRect("player_21.png"))
  , m_upRect(atlas.getTextureRect("player_24.png"))
  , m_level(level)
  , m_boxes(boxes)
  , m_prevDirection(gf::Direction::Down)
  , m_currDirection(gf::Direction::Center)
  {
  }

  void Hero::setData(const Data& data) {
    auto size = data.getSize();

    for (int y = 0; y < size.height; ++y) {
      for (int x = 0; x < size.width; ++x) {
        auto coords = gf::vec(x, y);
        char c = data(coords);

        if (c == '@' || c == '+') {
          m_position = coords;
        }
      }
    }
  }

  void Hero::setDirection(gf::Direction direction) {
    m_currDirection = direction;
  }

  void Hero::update(gf::Time time) {
    if (m_currDirection != gf::Direction::Center) {
      gf::Vector2i target = m_position + gf::displacement(m_currDirection);

      if (!m_level.isWall(target)) {
        if (m_boxes.hasBox(target)) {
          gf::Vector2i behind = target + gf::displacement(m_currDirection);

          if (!m_level.isWall(behind) && !m_boxes.hasBox(behind)) {
            m_position = target;
            m_boxes.changeBox(target, behind);
          }
        } else {
          m_position = target;
        }
      }

      m_prevDirection = m_currDirection;
    }

    m_currDirection = gf::Direction::Center;
  }

  void Hero::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Sprite sprite(m_texture);

    switch (m_prevDirection) {
      case gf::Direction::Left:
        sprite.setTextureRect(m_leftRect);
        break;
      case gf::Direction::Right:
        sprite.setTextureRect(m_rightRect);
        break;
      case gf::Direction::Down:
        sprite.setTextureRect(m_downRect);
        break;
      case gf::Direction::Up:
        sprite.setTextureRect(m_upRect);
        break;
      default:
        assert(false);
        break;
    }

    sprite.setPosition(m_position * 128.0f + 64.0f);
    sprite.setAnchor(gf::Anchor::Center);
    target.draw(sprite, states);
  }

}
