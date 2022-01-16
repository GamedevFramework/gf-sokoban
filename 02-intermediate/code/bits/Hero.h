#ifndef SKB_HERO_H
#define SKB_HERO_H

#include <gf/Direction.h>
#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/TextureAtlas.h>

#include "Data.h"

namespace skb {

  class Level;
  class Boxes;

  class Hero : public gf::Entity {
  public:
    Hero(const gf::TextureAtlas& atlas, const Level& level, Boxes& boxes);

    void setData(const Data& data);

    void setDirection(gf::Direction direction);

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const gf::Texture& m_texture;
    gf::RectF m_leftRect;
    gf::RectF m_rightRect;
    gf::RectF m_downRect;
    gf::RectF m_upRect;

    const Level& m_level;
    Boxes& m_boxes;

    gf::Direction m_prevDirection;
    gf::Direction m_currDirection;
    gf::Vector2i m_position;
  };

}

#endif // SKB_HERO_H
