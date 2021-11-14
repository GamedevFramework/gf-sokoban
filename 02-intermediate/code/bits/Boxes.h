#ifndef SKB_BOXES_H
#define SKB_BOXES_H

#include <gf/Entity.h>

namespace skb {

  class Boxes : public gf::Entity {
  public:

    void update(gf::Time time) override;
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  };

}

#endif // SKB_BOXES_H
