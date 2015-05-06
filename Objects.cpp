#include "Objects.h"

void Light(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + (tmx_obj->GetWidth()/32.)/2.0, go->getPosition().y + (tmx_obj->GetHeight()/32.)/2.0);
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d((tmx_obj->GetWidth()/32.), (tmx_obj->GetHeight()/32.)),
		hb::Vector2d(),
		tmx_obj->GetRot(),
		true);
	rigid_body->addCollider(collider);
	go->addComponent(rigid_body);
}


void Wall(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + (tmx_obj->GetWidth()/32.)/2.0, go->getPosition().y + (tmx_obj->GetHeight()/32.)/2.0);
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d((tmx_obj->GetWidth()/32.), (tmx_obj->GetHeight()/32.)),
		hb::Vector2d(),
		tmx_obj->GetRot(),
		false);
	rigid_body->addCollider(collider);
	go->addComponent(rigid_body);
}


void ApearingWall(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + (tmx_obj->GetWidth()/32.)/2.0, go->getPosition().y + (tmx_obj->GetHeight()/32.)/2.0);
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d((tmx_obj->GetWidth()/32.), (tmx_obj->GetHeight()/32.)),
		hb::Vector2d(),
		tmx_obj->GetRot(),
		false);
	rigid_body->addCollider(collider);
	go->addComponent(rigid_body);

	hb::Texture tex = hb::Texture::loadFromFile("res/drawable/tileset.png");
	hb::Sprite sprite(tex, hb::Vector2d(32, 32));
	sprite.setCenter(hb::Vector2d(0, 16));
	for (int j = 0; j < tmx_obj->GetHeight()/32.; ++j)
		for (int i = 0; i < tmx_obj->GetWidth()/32.; ++i)
		{
			auto spr = new hb::SpriteComponent(sprite, {50, 51, 52}, hb::Time::seconds(0.2));
			spr->setPosition(hb::Vector3d(i - tmx_obj->GetWidth()/32./2., j - tmx_obj->GetHeight()/32./2. + 0.5, 0));
			go->addComponent(spr);
		}

	bool b = (tmx_obj->GetProperties().GetStringProperty("start_active") == "true");
	go->setActive(b);

	auto fc = new hb::FunctionComponent;
	go->addComponent(fc);

	fc->addListener("enable", [=](hb::DataRepository&)
	{
		go->setActive(not go->isActive());
	});

	fc->addListener("disable", [=](hb::DataRepository&)
	{
		go->setActive(not go->isActive());
	});
}


void MovingWall(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + (tmx_obj->GetWidth()/32.)/2.0, go->getPosition().y + (tmx_obj->GetHeight()/32.)/2.0);
	body_def.type = b2_kinematicBody;
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d((tmx_obj->GetWidth()/32.), (tmx_obj->GetHeight()/32.)),
		hb::Vector2d(),
		tmx_obj->GetRot(),
		false);
	rigid_body->addCollider(collider);
	b2Fixture* fixture = rigid_body->getB2Body()->GetFixtureList();
	hb_assert(fixture != nullptr, "fixture no esta");
	go->addComponent(rigid_body);

	auto data = new MovingWallData;
	data->point[0].x = tmx_obj->GetProperties().GetFloatProperty("p0_x", body_def.position.x);
	data->point[0].y = tmx_obj->GetProperties().GetFloatProperty("p0_y", body_def.position.y);
	data->point[1].x = tmx_obj->GetProperties().GetFloatProperty("p1_x", data->point[0].x);
	data->point[1].y = tmx_obj->GetProperties().GetFloatProperty("p1_y", data->point[0].y);

	hb::Texture tex = hb::Texture::loadFromFile("res/drawable/tileset.png");
	hb::Sprite sprite(tex, hb::Vector2d(32, 32));
	sprite.setCenter(hb::Vector2d(0, 16));
	for (int i = 0; i < tmx_obj->GetWidth()/32.; ++i)
	{
		auto spr = new hb::SpriteComponent(sprite, {50, 51, 52}, hb::Time::seconds(0.2));
		spr->setPosition(hb::Vector3d(i - tmx_obj->GetWidth()/32./2., 0, 0));
		go->addComponent(spr);
	}

	double speed = tmx_obj->GetProperties().GetFloatProperty("speed", 1.);

	data->moving = (tmx_obj->GetProperties().GetStringProperty("start_moving") == "true");

	auto fc = new hb::FunctionComponent;
	go->addComponent(fc);

	fc->addListener("update", [=](hb::DataRepository& data_repo)
	{
		int i = data->iteration % 2;
		if(not data->moving) return;
		else if (rigid_body->getB2Body()->GetLinearVelocity() == b2Vec2())
		{
			hb::Vector2d dir = (data->point[i] - go->getPosition().xy()).normalized() * speed;
			rigid_body->getB2Body()->SetLinearVelocity(b2Vec2(dir.x, dir.y));
		}
		b2Vec2 p(data->point[i].x, data->point[i].y);
		if (fixture->TestPoint(p))
		{
			if (data->point[0] != data->point[1])
			{
				++data->iteration;
				hb::Vector2d dir = (data->point[(i+1)%2] - go->getPosition().xy()).normalized() * speed;
				rigid_body->getB2Body()->SetLinearVelocity(b2Vec2(dir.x, dir.y));
			}
			else
			{
				rigid_body->getB2Body()->SetLinearVelocity(b2Vec2());	
			}
		}
	});

	fc->addListener("enable", [=](hb::DataRepository&)
	{
		hb::Vector2d dir = (data->point[data->iteration%2] - go->getPosition().xy()).normalized() * speed;
		rigid_body->getB2Body()->SetLinearVelocity(b2Vec2(dir.x, dir.y));
		data->moving = true;
	});

	fc->addListener("disable", [=](hb::DataRepository&)
	{
		data->moving = false;
		rigid_body->getB2Body()->SetLinearVelocity(b2Vec2());
	});

	fc->addListener("destroy", [=](hb::DataRepository&)
	{
		delete data;
	});
}


void Trap(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + (tmx_obj->GetWidth()/32.)/2.0, go->getPosition().y + (tmx_obj->GetHeight()/32.)/2.0);
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d((tmx_obj->GetWidth()/32.), (tmx_obj->GetHeight()/32.)),
		hb::Vector2d(),
		tmx_obj->GetRot(),
		true);
	rigid_body->addCollider(collider);
	go->addComponent(rigid_body);
}


void Switch(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + .5, go->getPosition().y + .5);
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d(.6, 1.),
		hb::Vector2d(0, 0),
		tmx_obj->GetRot(),
		true);
	rigid_body->addCollider(collider);
	go->addComponent(rigid_body);

	auto data = new SwitchData;
	std::string actors = tmx_obj->GetProperties().GetStringProperty("actors");
	std::stringstream ss;
	ss << actors;
	int actor;
	while(ss >> actor)
	{
		data->actors.push_back(actor);
	}
	hb::Texture tex = hb::Texture::loadFromFile("res/drawable/tileset.png");
	hb::Sprite sprite(tex, hb::Vector2d(32, 32));
	sprite.setCenter(hb::Vector2d(16., 16.));
	auto spr_comp = new hb::SpriteComponent(sprite, {tmx_obj->GetGid()-1});

	auto fc = new hb::FunctionComponent;
	go->addComponents({fc, spr_comp});
	fc->addListener("switch", [=](hb::DataRepository& d)
	{
		hb::Vector3d s = spr_comp->getScale();
		s.x *= -1;
		spr_comp->setScale(s);

		if (data->is_active)
		{
			for(int actor_id : data->actors)
			{
				hb::GameObject::getGameObjectById(actor_id)->sendMessage("disable", d);
				data->is_active = false;
			}
		}
		else
		{
			for(int actor_id : data->actors)
			{
				hb::GameObject::getGameObjectById(actor_id)->sendMessage("enable", d);
				data->is_active = true;
			}
		}
	});

	fc->addListener("destroy", [=](hb::DataRepository&)
	{
		delete data;
	});
}


void Door(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + .5, go->getPosition().y + .5);
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d(0.3, 1.),
		hb::Vector2d(0.1, 0),
		tmx_obj->GetRot(),
		false);
	rigid_body->addCollider(collider);
	go->addComponent(rigid_body);

	hb::Texture tex = hb::Texture::loadFromFile("res/drawable/tileset.png");
	hb::Sprite sprite(tex, hb::Vector2d(32, 32));
	sprite.setCenter(hb::Vector2d(16., 16.));
	auto spr_comp = new hb::SpriteComponent(sprite, {63});

	auto fc = new hb::FunctionComponent;
	go->addComponents({fc, spr_comp});
	fc->addListener("enable", [=](hb::DataRepository&)
	{
		go->setActive(false);
	});

	fc->addListener("disable", [=](hb::DataRepository&)
	{
		go->setActive(true);
	});
}


void Memory(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x, go->getPosition().y);
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 0.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d(1., 1.),
		hb::Vector2d(0.5,0.5),
		tmx_obj->GetRot(),
		true);
	rigid_body->addCollider(collider);
	go->addComponent(rigid_body);

	hb::Texture tex = hb::Texture::loadFromFile("res/drawable/tileset.png");
	hb::Sprite sprite(tex, hb::Vector2d(32, 32));
	auto spr_comp = new hb::SpriteComponent(sprite, {61});
	sprite.setCenter(hb::Vector2d(16., 16.));
	go->addComponent(spr_comp);

	std::string scene = tmx_obj->GetProperties().GetStringProperty("next_scene");
	auto data = new MemoryData;
	auto fc = new hb::FunctionComponent;
	go->addComponent(fc);
	fc->addListener("sensorStart", [=](hb::DataRepository& collision)
	{
		if (collision.getPointer<hb::GameObject>("other")->getName() == "Player")
		{
			data->touching_player = true;
			spr_comp->setFrameOrder({60});
		}
	});

	fc->addListener("sensorEnd", [=](hb::DataRepository& collision)
	{
		if (collision.getPointer<hb::GameObject>("other")->getName() == "Player")
		{
			data->touching_player = false;
			spr_comp->setFrameOrder({61});
		}
	});

	auto listener = hb::InputManager::instance()->listen([=](hb::KeyPressed& e)
	{
		if (e.code == hb::Keyboard::Key::W and data->touching_player)
			hb::Game::setScene(scene);
	});


	fc->addListener("destroy", [=](hb::DataRepository& collision)
	{
		hb::InputManager::instance()->ignore(listener);
	});
}


void GameOver(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	auto data = new GameOverData;
	std::string scene = tmx_obj->GetProperties().GetStringProperty("next_scene");
	auto fc = new hb::FunctionComponent;
	go->addComponent(fc);
	fc->addListener("update", [=](hb::DataRepository& collision)
	{
		if (data->player == nullptr)
			data->player = hb::GameObject::getGameObjectsByName("Player")[0];
		if (data->player->getPosition().y > 100.)
			hb::Game::setScene(scene);
	});
}


void Player(hb::GameObject* go, const Tmx::Map* map, int obj_grp, int obj_id)
{
	const Tmx::Object* tmx_obj = map->GetObjectGroup(obj_grp)->GetObject(obj_id);
	b2BodyDef body_def;
	body_def.position = b2Vec2(go->getPosition().x + .5, go->getPosition().y + .5);
	body_def.type = b2_dynamicBody;
	body_def.fixedRotation = true;
	auto rigid_body = new hb::RigidBody2dComponent(body_def);
	hb::Material2d mat;
	mat.friction = tmx_obj->GetProperties().GetFloatProperty("friction", 100000.);
	mat.bounciness = tmx_obj->GetProperties().GetFloatProperty("bounciness", 0.);
	mat.density = tmx_obj->GetProperties().GetFloatProperty("density", 1.);
	hb::BoxCollider2d collider(
		mat,
		hb::Vector2d(0.5, 0.96),
		hb::Vector2d(0, 0.0),
		0.,
		false);
	rigid_body->addCollider(collider);
	hb::BoxCollider2d grounded_sensor(
		mat,
		hb::Vector2d(0.38, 0.2),
		hb::Vector2d(0, 0.5),
		0.,
		true);

	unsigned int grounded_sensor_id = rigid_body->addCollider(grounded_sensor);
	go->addComponent(rigid_body);

	auto data = new PlayerData;
	int gid = tmx_obj->GetGid() - 1;
	data->gid = gid;
	if (gid == 30)
	{
		data->go_right_frame_order[0] = gid;
		data->go_right_frame_order[1] = gid + 1;
		data->go_right_frame_order[2] = gid;
		data->go_right_frame_order[3] = gid + 2;
		gid += 10;
		data->go_left_frame_order[0] = gid;
		data->go_left_frame_order[1] = gid + 1;
		data->go_left_frame_order[2] = gid;
		data->go_left_frame_order[3] = gid + 2;
	}
	else
	{
		data->go_right_frame_order[0] = gid;
		data->go_right_frame_order[1] = gid + 10;
		data->go_right_frame_order[2] = gid;
		data->go_right_frame_order[3] = gid + 20;
		++gid;
		data->go_left_frame_order[0] = gid;
		data->go_left_frame_order[1] = gid + 10;
		data->go_left_frame_order[2] = gid;
		data->go_left_frame_order[3] = gid + 20;
	}

	hb::Texture tex = hb::Texture::loadFromFile("res/drawable/tileset.png");
	hb::Sprite sprite(tex, hb::Vector2d(32, 32));
	sprite.setCenter(hb::Vector2d(16., 16.));
	auto spr_comp = new hb::SpriteComponent(sprite, {data->go_right_frame_order[0]}, hb::Time::seconds(0.2));
	go->addComponent(spr_comp);

	data->player_sprite = spr_comp;

	auto fc = new hb::FunctionComponent;
	go->addComponent(fc);
	fc->addListener("update", [=](hb::DataRepository&)
	{
		if (data->player_vel.module() > 0)
			data->player_sprite->play();
		else
			data->player_sprite->stop();

		b2Vec2 v = rigid_body->getB2Body()->GetLinearVelocity();
		b2Vec2 v_ = b2Vec2(data->player_vel.x, data->player_vel.y);
		v.y += v_.y;
		v.x = data->platform_vel.x + v_.x;
		rigid_body->getB2Body()->SetLinearVelocity(v);

		hb::Renderer::getCamera().setPosition(go->getPosition());
	});

	fc->addListener("sensorStart", [=](hb::DataRepository& collision)
	{
		auto otherFixture = collision.getPointer<b2Fixture>("otherFixture");
		if (collision.getPointer<hb::Fixture2dData>("fixtureData")->id == grounded_sensor_id
			and not otherFixture->IsSensor())
			++data->grounded_count;
		else if (otherFixture->IsSensor() and collision.getPointer<hb::GameObject>("other")->getName() == "Light")
		{
			int gid = 0;
			data->go_right_frame_order[0] = gid;
			data->go_right_frame_order[1] = gid + 10;
			data->go_right_frame_order[2] = gid;
			data->go_right_frame_order[3] = gid + 20;
			++gid;
			data->go_left_frame_order[0] = gid;
			data->go_left_frame_order[1] = gid + 10;
			data->go_left_frame_order[2] = gid;
			data->go_left_frame_order[3] = gid + 20;
			if (data->player_vel.x < 0)
				data->player_sprite->setFrameOrder(data->go_left_frame_order);
			else
				data->player_sprite->setFrameOrder(data->go_right_frame_order);
		}
		else if (otherFixture->IsSensor() and collision.getPointer<hb::GameObject>("other")->getName() == "SemiLight")
		{
			int gid = 2;
			data->go_right_frame_order[0] = gid;
			data->go_right_frame_order[1] = gid + 10;
			data->go_right_frame_order[2] = gid;
			data->go_right_frame_order[3] = gid + 20;
			++gid;
			data->go_left_frame_order[0] = gid;
			data->go_left_frame_order[1] = gid + 10;
			data->go_left_frame_order[2] = gid;
			data->go_left_frame_order[3] = gid + 20;
			if (data->player_vel.x < 0)
				data->player_sprite->setFrameOrder(data->go_left_frame_order);
			else
				data->player_sprite->setFrameOrder(data->go_right_frame_order);
		}
	});

	fc->addListener("sensorStay", [=](hb::DataRepository& data_repo)
	{
		auto fixt_data = data_repo.getPointer<hb::Fixture2dData>("fixtureData");
		auto other = data_repo.getPointer<hb::GameObject>("other");
		auto other_fixture = data_repo.getPointer<b2Fixture>("otherFixture");
		if (fixt_data->id == grounded_sensor_id)
		{
			if (other->getName() == "MovingWall")
			{
				b2Vec2 vo = other_fixture->GetBody()->GetLinearVelocity();
				vo.y = 0;
				data->platform_vel = hb::Vector2d(vo.x, vo.y);
			}
			else if (other->getName() == "Trap")
			{
				hb::Game::setScene(hb::Game::getCurrentScene());
			}
		}
	});

	fc->addListener("sensorEnd", [=](hb::DataRepository& collision)
	{
		auto otherFixture = collision.getPointer<b2Fixture>("otherFixture");
		if (collision.getPointer<hb::Fixture2dData>("fixtureData")->id == grounded_sensor_id
			and not otherFixture->IsSensor())
		{
			data->platform_vel = hb::Vector2d();
			--data->grounded_count;
		}
		else if (otherFixture->IsSensor() and
			(collision.getPointer<hb::GameObject>("other")->getName() == "Light"
			or collision.getPointer<hb::GameObject>("other")->getName() == "SemiLight"))
		{
			int gid = data->gid;
			if (gid == 30)
			{
				data->go_right_frame_order[0] = gid;
				data->go_right_frame_order[1] = gid + 1;
				data->go_right_frame_order[2] = gid;
				data->go_right_frame_order[3] = gid + 2;
				gid += 10;
				data->go_left_frame_order[0] = gid;
				data->go_left_frame_order[1] = gid + 1;
				data->go_left_frame_order[2] = gid;
				data->go_left_frame_order[3] = gid + 2;
			}
			else
			{
				data->go_right_frame_order[0] = gid;
				data->go_right_frame_order[1] = gid + 10;
				data->go_right_frame_order[2] = gid;
				data->go_right_frame_order[3] = gid + 20;
				++gid;
				data->go_left_frame_order[0] = gid;
				data->go_left_frame_order[1] = gid + 10;
				data->go_left_frame_order[2] = gid;
				data->go_left_frame_order[3] = gid + 20;
			}
			if (data->player_vel.x < 0)
				data->player_sprite->setFrameOrder(data->go_left_frame_order);
			else
				data->player_sprite->setFrameOrder(data->go_right_frame_order);
		}
	});

	fc->addListener("collisionStay", [=](hb::DataRepository& collision)
	{
		auto other = collision.getPointer<hb::GameObject>("other");
		if (other->getName() == "Switch")
		{
			data->current_switch = other;
		}
	});

	fc->addListener("collisionEnd", [=](hb::DataRepository& collision)
	{
		auto other = collision.getPointer<hb::GameObject>("other");
		if (other->getName() == "Switch")
		{
			data->current_switch = nullptr;
		}
	});

	hb::InputManager::ListenerId<hb::KeyPressed> keypressed_listener_id;
	hb::InputManager::ListenerId<hb::KeyReleased> keyreleased_listener_id;

	keypressed_listener_id = hb::InputManager::instance()->listen(
	[=](hb::KeyPressed& event)
	{
		double value = 5.5;

		hb::Keyboard::Key code = event.code;
		if (code == hb::Keyboard::Key::Space)
		{
			if (data->grounded_count > 0)
			{
				b2Vec2 v = rigid_body->getB2Body()->GetLinearVelocity();
				v.y = -12.5;
				rigid_body->getB2Body()->SetLinearVelocity(v);
			}
		}
		else if (code == hb::Keyboard::Key::A and data->player_vel.x >= 0)
		{
			data->player_vel.x = -value;
			data->player_sprite->setFrameOrder(data->go_left_frame_order);
		}
		else if (code == hb::Keyboard::Key::D and data->player_vel.x <= 0)
		{
			data->player_vel.x = value;
			data->player_sprite->setFrameOrder(data->go_right_frame_order);
		}
		else if (code == hb::Keyboard::Key::W)
		{
			if (data->current_switch != nullptr)
			{
				hb::DataRepository d;
				data->current_switch->sendMessage("switch", d);
			}
		}
	});
	keyreleased_listener_id = hb::InputManager::instance()->listen(
	[=](hb::KeyReleased& event)
	{
		hb::Keyboard::Key code = event.code;
		if (code == hb::Keyboard::Key::A and data->player_vel.x < 0)
		{
			data->player_vel.x = 0;
			data->player_sprite->setFrameOrder({data->go_left_frame_order[0]});
		}
		else if (code == hb::Keyboard::Key::D and data->player_vel.x > 0)
		{
			data->player_vel.x = 0;
			data->player_sprite->setFrameOrder({data->go_right_frame_order[0]});
		}
		else if (code == hb::Keyboard::Key::W)
		{
		}
	});

	fc->addListener("destroy", [=](hb::DataRepository&)
	{
		hb::InputManager::instance()->ignore(keypressed_listener_id);
		hb::InputManager::instance()->ignore(keyreleased_listener_id);
		delete data;
	});
}


void DisplayMemory(const std::string& memory_path, const std::string& next_scene)
{
	auto go = new hb::GameObject();
	hb::Texture blank = hb::Texture::makeTexture(hb::Vector2d(1,1));
	blank.fill(hb::Rect(0, 0, 1, 1), hb::Color(1.f, 1.f, 1.f));
	blank.repeat(true);
	auto spr_blank = hb::Sprite(blank);
	spr_blank.setFrameSize(hb::Vector2d(1200, 720));
	auto sprite_blank = new hb::SpriteComponent(spr_blank);
	sprite_blank->setPosition(0, 0, 100);

	hb::Texture memory = hb::Texture::loadFromFile(memory_path);
	auto sprite_memory = new hb::SpriteComponent(hb::Sprite(memory));

	hb::Texture text = hb::Texture::loadFromFile("res/drawable/tileset.png", hb::Rect(224, 224, 96, 32));
	auto sprite_text = new hb::SpriteComponent(hb::Sprite(text));
	hb::Vector2d w_size;
	w_size.x = hb::Renderer::getWindow().getView().getSize().x;
	w_size.y = hb::Renderer::getWindow().getView().getSize().y;
	sprite_text->setPosition(hb::Vector3d(w_size.x/2 - 96/4, w_size.y - 32, 50));
	sprite_text->setColor(hb::Color(0,0,0,0));
	sprite_text->setScale(hb::Vector3d(0.5, 0.5, 1.));

	auto fc = new hb::FunctionComponent();
	go->addComponents({sprite_blank, sprite_memory, sprite_text, fc});

	auto data = new DisplayMemoryData;
	data->time = hb::Time::seconds(1.5);
	fc->addListener("update", [=](hb::DataRepository& d)
	{
		data->time -= hb::Time::deltaTime;
		switch(data->status){
			case 0:
				if (data->time.asSeconds() < 0)
				{
					sprite_blank->setColor(hb::Color(0.f, 0.f, 0.f, 0.f));
					data->status = 1;
				}
				else
				{
					sprite_blank->setColor(hb::Color(1.f, 1.f, 1.f, data->time.asSeconds()/1.5f));
				}
				break;
			case 1:
				if (data->time.asSeconds() < -1.5 and next_scene != "")
				{
					sprite_text->setColor(hb::Color(1.f, 1.f, 1.f, 1.f));
				}
				break;
			case 2:
				if (data->time.asSeconds() < 0)
				{
					sprite_blank->setColor(hb::Color(0.f, 0.f, 0.f, 1.f));
					hb::Game::setScene(next_scene);
				}
				else
				{
					sprite_blank->setColor(hb::Color(0.f, 0.f, 0.f, 1.f - data->time.asSeconds()/1.5f));
				}
				break;
		}
	});

	auto keypressed_listener_id = hb::InputManager::instance()->listen(
	[=](hb::KeyPressed& event)
	{
		if (data->status == 1 and next_scene != "")
		{
			data->time = hb::Time::seconds(1.5);
			data->status = 2;
		}
	});

	fc->addListener("destroy", [=](hb::DataRepository&)
	{
		hb::InputManager::instance()->ignore(keypressed_listener_id);
		delete data;
	});
}
