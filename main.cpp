#include "HB/Core.h"
#include "HB/SFML.h"
#include "HB/Box2D.h"
#include "HB/Tmx/TmxScene.h"
#include "B2dDebugDrawPlugin.h"
#include "Objects.h"

void camMemories()
{
	hb::Renderer::getCamera().setAxisX(hb::Vector3d(1, 0, 0));
	hb::Renderer::getCamera().setAxisY(hb::Vector3d(0, 1, 0));
	hb::Renderer::getCamera().setAxisZ(hb::Vector3d(0, 0, 1));

	int width = 600;// 1200;
	int height = 360;// 720;
	hb::Renderer::getCamera().setPosition(hb::Vector2d(width/2, height/2));
	hb::Renderer::getWindow().setView(sf::View(sf::FloatRect(0, 0, width, height)));
	hb::Renderer::setClearColor(hb::Color(1.f, 1.f, 1.f));
}

void camLevels(const Tmx::Map* map)
{
	hb::Renderer::getCamera().setPosition(hb::Vector2d(map->GetWidth()/2., map->GetHeight()/2.));
	hb::Renderer::getWindow().setView(sf::View(sf::FloatRect(0, 0, 600, 360)));
}

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
	hb::TmxRegisterFactory("SemiLight", Light);
	hb::TmxRegisterFactory("Wall", Wall);
	hb::TmxRegisterFactory("ApearingWall", ApearingWall);
	hb::TmxRegisterFactory("MovingWall", MovingWall);
	hb::TmxRegisterFactory("Switch", Switch);
	hb::TmxRegisterFactory("Door", Door);
	hb::TmxRegisterFactory("Memory", Memory);
	hb::TmxRegisterFactory("GameOver", GameOver);
	hb::TmxRegisterFactory("Player", Player);
	hb::TmxRegisterFactory("Trap", Trap);

	hb::Physics2d::getB2World()->SetGravity(b2Vec2(0, 50));

	//hb::Game::addScene(hb::TmxScene("demo", "res/levels/demo.tmx", cam));
	hb::Game::addScene(hb::TmxScene("lvl1", "res/levels/lvl1.tmx", camLevels));
	hb::Game::addScene(hb::TmxScene("lvl2", "res/levels/lvl2.tmx", camLevels));
	hb::Game::addScene(hb::TmxScene("lvl3", "res/levels/lvl3.tmx", camLevels));
	hb::Game::addScene(hb::TmxScene("lvl4", "res/levels/lvl4.tmx", camLevels));
	hb::Game::addScene(hb::Game::Scene("memory1", [](){
		camMemories();
		DisplayMemory("res/drawable/memory1.png", "lvl2");
	}));
	hb::Game::addScene(hb::Game::Scene("memory2", [](){
		camMemories();
		DisplayMemory("res/drawable/memory2.png", "lvl3");
	}));
	hb::Game::addScene(hb::Game::Scene("memory3", [](){
		camMemories();
		DisplayMemory("res/drawable/tileset.png", "lvl4");
	}));
	hb::Game::addScene(hb::Game::Scene("memory4", [](){
		camMemories();
		DisplayMemory("res/drawable/tileset.png", "credits");
	}));
	hb::Game::addScene(hb::Game::Scene("memory4.1", [](){
		camMemories();
		DisplayMemory("res/drawable/tileset.png", "credits");
	}));

	hb::Game::addScene(hb::TmxScene("credits", "res/levels/credits.tmx", camLevels));

	// Start Game
	hb::Game::setScene("lvl1");
	hb::Game::run();

	return 0;
}
