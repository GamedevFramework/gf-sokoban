#include "Level.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "Boxes.h"

namespace skb {

  namespace {

    constexpr gf::Vector2f SpriteSize(128.0f, 128.0f);

  }

  Level::Level(const gf::TextureAtlas& atlas, const Boxes& boxes)
  : gf::Entity(1)
  , m_texture(atlas.getTexture())
  , m_groundRect(atlas.getTextureRect("ground_03.png"))
  , m_wallRect(atlas.getTextureRect("block_02.png"))
  , m_goalRect(atlas.getTextureRect("environment_08.png"))
  , m_boxes(boxes)
  {
  }

  void Level::setData(const Data& data) {
    gf::Array2D<Block, int> blocks(data.getSize());

    for (auto coords : blocks.getPositionRange()) {
      switch (data(coords)) {
        case ' ':
        case '@':
        case '$':
          blocks(coords) = Block::Floor;
          break;
        case '.':
        case '+':
        case '*':
          blocks(coords) = Block::Goal;
          break;
        case '#':
          blocks(coords) = Block::Wall;
          break;
        default:
          assert(false);
          break;
      }
    }

    m_blocks = std::move(blocks);
  }

  bool Level::isWall(gf::Vector2i coords) const {
    return m_blocks(coords) == Block::Wall;
  }

  bool Level::isFinished() const {
    for (auto coords : m_blocks.getPositionRange()) {
      if (m_blocks(coords) == Block::Goal) {
        if (!m_boxes.hasBox(coords)) {
          return false;
        }
      }
    }

    return true;
  }

  void Level::update(gf::Time time) {

  }

  void Level::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto coords : m_blocks.getPositionRange()) {
      gf::Sprite sprite(m_texture);
      sprite.setPosition(coords * SpriteSize);
      sprite.setTextureRect(m_groundRect);
      target.draw(sprite, states);
    }

    for (auto coords : m_blocks.getPositionRange()) {
      gf::Sprite sprite(m_texture);

      switch (m_blocks(coords)) {
        case Block::Goal:
          sprite.setTextureRect(m_goalRect);
          sprite.setPosition(coords * SpriteSize + SpriteSize * 0.5f);
          sprite.setAnchor(gf::Anchor::Center);
          break;
        case Block::Wall:
          sprite.setTextureRect(m_wallRect);
          sprite.setPosition(coords * SpriteSize);
          sprite.setAnchor(gf::Anchor::TopLeft);
          break;
        default:
          continue;
      }

      target.draw(sprite, states);
    }
  }

}

