#pragma once
#include "LibDosesWin.h"

namespace d2Doses1
{
	// PRE DEFINITIONS
	class container;
	class object;

	// useful classes
	enum class objectTypes
	{
		rect, ellipse, line
	};
	enum class brushTypes
	{
		solid, linear, radial, bitmap
	};
	enum class mouseEvent
	{
		leftDown,			leftUp, 
		rightDown,			rightUp, 
		middleDown,			middleUp, 
		middleLeftDown,		middleLeftUp, 
		middleRightDown,	middleRightUp, 
		wheelDown,			wheelUp, 
		leftDouble,			rightDouble, 
		leftDrag,			rightDrag, 
		hoverOn,			hoverOff,
		move
	};
	struct keyEvent
	{
		bool down[26];
		bool held[26];
		bool   up[26];
	};

	static class DEFS	// Holds default values for various things, as well as the prefered container
	{

	public:
		container* container;

	}DEFAULTS;

	class container
	{
		dynArray<object*> m_objs;
		ID2D1HwndRenderTarget* pRT;
	public:
		// tors
		container() : pRT(NULL) {};

		void create(ID2D1HwndRenderTarget* RT);

		int add(object* newObject);

	};

	class object
	{
		objectTypes m_type;				// What type the object is
		object* m_parent;				// The parent of the object
		dynArray<object*> m_children;	// The children of the object

		int id;

	protected:
		object() {};
		object(objectTypes Type);	// init with object type

		virtual bool inBounds(int X, int Y) { return 0; };
	public:
		int reOrder(int Offset);
		void move(float X, float Y);
		void setPos(float X, float Y);

		virtual void eventSysyem();
	};

	class rect : public object
	{
		RECT m_rect;	// The visual size of the rectangle
		RECT m_bounds;	// The interaction size of the rectangle
		float m_originX;
		float m_originY;	// Origin is where the object will scale from
		float m_scaleX;
		float m_scaleY;		// Scale changes the objects size without writing to its size
		float m_rotation;

	public:
		rect() {};

	};
}

#include "d2Doses1.cpp"

/*
events:

Available Vars:
MouseX
MouseY
Key(s)

Available Commands:
CaptureMouse
ReleaseMouse
CaptureObject
ReleaseObject

Mouse:
OnLeftDown
OnLeftUp
OnRightDown
OnRightUp
OnMiddleDown
OnMiddleUp
OnMiddleLeftDown
OnMiddleLeftUp
OnMiddleRightDown
OnMiddleRightUp
OnWheelDown
OnWheelUp
OnHoverOn
OnHoverOff

*/