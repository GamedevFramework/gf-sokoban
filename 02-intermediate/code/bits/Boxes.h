#ifndef SKB_BOXES_H
#define SKB_BOXES_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/TextureAtlas.h>

#include "Data.h"

namespace skb {

  class Boxes : public gf::Entity {
  public:
    Boxes(const gf::TextureAtlas& atlas);

    void setData(const Data& data);

    bool hasBox(gf::Vector2i coordinates) const;
    void changeBox(gf::Vector2i from, gf::Vector2i to);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const gf::Texture& m_texture;
    gf::RectF m_crateRect;

    std::vector<gf::Vector2i> m_positions;
  };

}

#endif // SKB_BOXES_H
