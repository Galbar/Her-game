#include "HB/Core.h"
#include "HB/SFML.h"
#include "HB/Box2D.h"
#include "HB/Tmx.h"

void Light(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void Wall(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void MovingWall(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void ApearingWall(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void Trap(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void Switch(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void Door(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void Memory(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);
void Player(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id);

struct MovingWallData
{
	bool moving = false;
	unsigned int iteration = 0;
	hb::Vector2d velocity;
	hb::Vector2d point[2];
	b2Fixture* fixture;

	MovingWallData():
	moving(false),
	iteration(0),
	velocity(),
	fixture(nullptr)
	{
		point[0] = hb::Vector2d();
		point[1] = hb::Vector2d();
	};
};


struct SwitchData
{
	bool is_active = false;
	std::vector<int> actors;
};


struct MemoryData
{
	bool touching_player = false;
};


struct PlayerData
{
	hb::Vector2d platform_vel;
	hb::Vector2d player_vel;
	unsigned int grounded_count = 0;
	hb::SpriteComponent* player_sprite = nullptr;
	hb::GameObject* current_switch = nullptr;
	std::vector<int> go_right_frame_order = std::vector<int>(4);
	std::vector<int> go_left_frame_order = std::vector<int>(4);
	int gid;
};
