#include "Boxes.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

namespace skb {

  Boxes::Boxes(const gf::TextureAtlas& atlas)
  : gf::Entity(2)
  , m_texture(atlas.getTexture())
  , m_crateRect(atlas.getTextureRect("crate_04.png"))
  {
  }

  void Boxes::setData(const Data& data) {
    m_positions.clear();
    auto size = data.getSize();

    for (int y = 0; y < size.height; ++y) {
      for (int x = 0; x < size.width; ++x) {
        auto coords = gf::vec(x, y);
        char c = data(coords);

        if (c == '$' || c == '*') {
          m_positions.push_back(coords);
        }
      }
    }
  }


  bool Boxes::hasBox(gf::Vector2i coordinates) const {
    for (auto position : m_positions) {
      if (position == coordinates) {
        return true;
      }
    }

    return false;
  }

  void Boxes::changeBox(gf::Vector2i from, gf::Vector2i to) {
    for (auto & position : m_positions) {
      if (position == from) {
        position = to;
        return;
      }
    }

    assert(false);
  }

  void Boxes::update(gf::Time time) {

  }

  void Boxes::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto position : m_positions) {
      gf::Sprite sprite(m_texture, m_crateRect);
      sprite.setPosition(position * 128.0f);
      target.draw(sprite, states);
    }
  }

}

