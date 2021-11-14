#ifndef SKB_LEVEL_H
#define SKB_LEVEL_H

#include <gf/Entity.h>

namespace skb {

  class Level : public gf::Entity {
  public:

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  };

}

#endif // SKB_HERO_H
