#ifndef SKB_LEVEL_H
#define SKB_LEVEL_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/TextureAtlas.h>

#include "Data.h"

namespace skb {

  class Boxes;

  class Level : public gf::Entity {
  public:
    Level(const gf::TextureAtlas& atlas, const Boxes& boxes);

    void setData(const Data& data);

    bool isWall(gf::Vector2i coords) const;

    bool isFinished() const;

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const gf::Texture& m_texture;
    gf::RectF m_groundRect;
    gf::RectF m_wallRect;
    gf::RectF m_goalRect;

    const Boxes& m_boxes;

    enum class Block {
      Wall,
      Goal,
      Floor,
    };

    gf::Array2D<Block, int> m_blocks;
  };

}

#endif // SKB_HERO_H
