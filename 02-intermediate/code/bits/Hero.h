#ifndef SKB_HERO_H
#define SKB_HERO_H

#include <gf/Entity.h>

namespace skb {

  class Hero : public gf::Entity {
  public:

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  };

}

#endif // SKB_HERO_H
