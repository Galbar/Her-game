#include "HB/Core.h"
#include "HB/SFML.h"
#include "HB/Box2D.h"
#include "HB/Tmx/TmxScene.h"
#include "B2dDebugDrawPlugin.h"
#include "Objects.h"


int main(int argc, char const *argv[])
{
	// Create Game window
	hb::Renderer::createWindow(hb::Vector2d(1200, 720), "Game");
	// Register SFML Plugin in Game (allows to update input and draw when necessary)
	hb::Game::addPlugin<hb::SFMLPlugin>();
	hb::Game::addPlugin<hb::Box2DPlugin>();
	//hb::Game::addPlugin<B2dDebugPlugin>();
	// Register Object factories for Tmx scenes
	hb::TmxRegisterFactory("Light", Light);
	hb::TmxRegisterFactory("Wall", Wall);
	hb::TmxRegisterFactory("ApearingWall", ApearingWall);
	hb::TmxRegisterFactory("MovingWall", MovingWall);
	hb::TmxRegisterFactory("Switch", Switch);
	hb::TmxRegisterFactory("Door", Door);
	hb::TmxRegisterFactory("Memory", Memory);
	hb::TmxRegisterFactory("Player", Player);
	hb::TmxRegisterFactory("Trap", Trap);

	hb::Physics2d::getB2World()->SetGravity(b2Vec2(0, 50));

	// Add Scenes loaded from Tmx file to game
	std::function<void(const Tmx::Map*)> cam = [](const Tmx::Map* map)
	{
		//hb::Renderer::getWindow().setSize(sf::Vector2u(map->GetWidth() * map->GetTileWidth(), map->GetHeight() * map->GetTileHeight()));
		hb::Renderer::getCamera().setPosition(hb::Vector2d(map->GetWidth()/2., map->GetHeight()/2.));
		hb::Renderer::getWindow().setView(sf::View(sf::FloatRect(0, 0, 600, 360)));
	};
	//hb::Game::addScene(hb::TmxScene("demo", "res/levels/demo.tmx", cam));
	hb::Game::addScene(hb::TmxScene("lvl1", "res/levels/lvl1.tmx", cam));
	hb::Game::addScene(hb::TmxScene("lvl2", "res/levels/lvl2.tmx", cam));
	hb::Game::addScene(hb::TmxScene("lvl3", "res/levels/lvl3.tmx", cam));
	hb::Game::addScene(hb::TmxScene("lvl4", "res/levels/lvl4.tmx", cam));

	// Start Game
	hb::Game::setScene("lvl1");
	hb::Game::run();

	return 0;
}
