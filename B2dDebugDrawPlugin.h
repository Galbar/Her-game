#ifndef B2D_DEBUG_DRAW_PLUGIN
#define B2D_DEBUG_DRAW_PLUGIN
#include "HB/Core.h"
#include "HB/SFML.h"
#include "HB/Box2D.h"

class B2dDebugDraw : public b2Draw
{
public:
	std::vector<sf::Drawable*> shapes;
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
};

class B2dDebugPlugin : public hb::Plugin
{
public:
	B2dDebugPlugin():m_debug_draw(){}
	~B2dDebugPlugin(){}
	
	void gameStart() override
	{
		hb::Physics2d::getB2World()->SetDebugDraw(&m_debug_draw);
		m_debug_draw.SetFlags( b2Draw::e_shapeBit);
	}

	void postUpdate() override
	{
		m_debug_draw.shapes.clear();
		hb::Physics2d::getB2World()->DrawDebugData();
	}
private:
	B2dDebugDraw m_debug_draw;
};
#endif
