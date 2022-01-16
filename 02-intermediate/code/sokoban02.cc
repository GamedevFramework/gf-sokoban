#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/TextureAtlas.h>
#include <gf/RenderWindow.h>
#include <gf/ResourceManager.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "bits/Boxes.h"
#include "bits/Data.h"
#include "bits/Hero.h"
#include "bits/Level.h"

#include "config.h"

namespace {

  const char Level1Data[] = {
    "  #####"
    "###@ .#"
    "# $ #.#"
    "#  $$ #"
    "#.  # #"
    "#   $.#"
    "#######"
  };

  constexpr gf::Vector2i Level1Size = gf::vec(7, 7);

}

int main() {
  static constexpr gf::Vector2i ScreenSize(800, 600);
  static constexpr gf::Vector2f ViewSize(100.0f, 100.0f);
  static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);

  // initialization

  gf::Window window("sokoban02", ScreenSize);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  gf::RenderWindow renderer(window);

  // ressources

  gf::ResourceManager resources;
  resources.addSearchDir(SOKOBAN_DATA_DIR);

  gf::TextureAtlas atlas("sokoban_spritesheet@2.xml", resources);

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialFramebufferSize(ScreenSize);

  // actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  actions.addAction(fullscreenAction);

  gf::Action leftAction("Left");
  leftAction.addScancodeKeyControl(gf::Scancode::A);
  leftAction.addScancodeKeyControl(gf::Scancode::Left);
  actions.addAction(leftAction);

  gf::Action rightAction("Right");
  rightAction.addScancodeKeyControl(gf::Scancode::D);
  rightAction.addScancodeKeyControl(gf::Scancode::Right);
  actions.addAction(rightAction);

  gf::Action upAction("Up");
  upAction.addScancodeKeyControl(gf::Scancode::W);
  upAction.addScancodeKeyControl(gf::Scancode::Up);
  actions.addAction(upAction);

  gf::Action downAction("Down");
  downAction.addScancodeKeyControl(gf::Scancode::S);
  downAction.addScancodeKeyControl(gf::Scancode::Down);
  actions.addAction(downAction);

  // entities

  skb::Data level1(Level1Data, Level1Size);

  skb::Data *currentLevel = &level1;

  gf::EntityContainer mainEntities;
  // add entities to mainEntities

  skb::Boxes boxes(atlas);
  boxes.setData(*currentLevel);
  mainEntities.addEntity(boxes);

  skb::Level level(atlas, boxes);
  level.setData(*currentLevel);
  mainEntities.addEntity(level);

  skb::Hero hero(atlas, level, boxes);
  hero.setData(*currentLevel);
  mainEntities.addEntity(hero);

  gf::EntityContainer hudEntities;
  // add entities to hudEntities

  // game loop

  renderer.clear(gf::Color::White);

  gf::Clock clock;

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (rightAction.isActive()) {
      hero.setDirection(gf::Direction::Right);
    } else if (leftAction.isActive()) {
      hero.setDirection(gf::Direction::Left);
    } else if (upAction.isActive()) {
      hero.setDirection(gf::Direction::Up);
    } else if (downAction.isActive()) {
      hero.setDirection(gf::Direction::Down);
    } else {
      // do something
    }


    // 2. update

    gf::Time time = clock.restart();
    mainEntities.update(time);
    hudEntities.update(time);


    // 3. draw

    renderer.clear();

    gf::Vector2f levelSize = currentLevel->getSize() * 128.0f;
    mainView.reset(gf::RectF::fromSize(levelSize));

    renderer.setView(mainView);
    mainEntities.render(renderer);

    renderer.setView(hudView);
    hudEntities.render(renderer);

    renderer.display();

    actions.reset();
  }

  return 0;
}
