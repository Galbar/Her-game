#include "HB/Core.h"
#include "HB/SFML.h"
#include "HB/Box2D.h"
#include "HB/Tmx/TmxScene.h"
#include "B2dDebugDrawPlugin.h"


int main(int argc, char const *argv[])
{
	// Create Game window
	hb::Renderer::createWindow(hb::Vector2d(960, 640), "Game");
	// Register SFML Plugin in Game (allows to update input and draw when necessary)
	hb::Game::addPlugin<hb::SFMLPlugin>();
	hb::Game::addPlugin<hb::Box2DPlugin>();
	hb::Game::addPlugin<B2dDebugPlugin>();
	// Register Object factories for Tmx scenes
	//hb::TmxRegisterFactory("Wall", makeWall);
	hb::Physics2d::getB2World()->SetGravity(b2Vec2(0, 10));

	// Add Scenes loaded from Tmx file to game
	//std::function<void(const Tmx::Map*)> cam = [](const Tmx::Map* map)
	//{
	//	hb::Renderer::getWindow().setSize(sf::Vector2u(map->GetWidth() * map->GetTileWidth(), map->GetHeight() * map->GetTileHeight()));
	//	hb::Renderer::getCamera().setPosition(hb::Vector2d(map->GetWidth()/2., map->GetHeight()/2.));
	//	hb::Renderer::getWindow().setView(sf::View(sf::FloatRect(0, 0, map->GetWidth() * map->GetTileWidth(), map->GetHeight() * map->GetTileHeight())));
	//};
	//hb::Game::addScene(hb::TmxScene("demo", "res/levels/demo.tmx", cam));

	// Start Game
	hb::Game::setScene("scene1");
	hb::Game::run();

	return 0;
}
