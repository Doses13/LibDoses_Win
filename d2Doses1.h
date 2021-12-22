#pragma once
#include "LibDosesWin.h"

namespace d2Doses1
{
	// Pre Definitions
	class container;
	class object;

	// Enum classes
	enum class objectType
	{
		rect, ellipse, line, geometry, custom
	};
	enum class brushType
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
		move,				nullEvent
	};
	struct keyEvent
	{
		bool down[26];
		bool held[26];
		bool   up[26];
		keyEvent()
		{
			for (int i = 0; i < 26; i++)
			{
				down[i] = held[i] = up[i] = 0;
			}
		}
	};
	struct MKEvent
	{
		D2D1_POINT_2L cords;
		mouseEvent button = mouseEvent::nullEvent;
		keyEvent keys;
	};

	// Brush classes
	class brush
	{
	protected:
		ID2D1Brush* m_brush = nullptr;
		brushType m_type = brushType::solid;
	public:
		brush();											// Default
		virtual ID2D1Brush* getPtr();						// Gets the 2d2 brush pointer
		virtual HRESULT create(ID2D1HwndRenderTarget*) = 0;	// Gets a value for the pointer		
	};

	class solidBrush : public brush
	{
		D2D1_COLOR_F m_color;	// Just used as a place holder between instantiation and creation of d2d brush
	public:
		solidBrush(float Red, float Green, float Blue, float Alpha = 1.0f);	// Init with seperate color values
		solidBrush(unsigned int RGB = 0xff0070, float Alpha = 1.0f);		// Init with an rgb number
		ID2D1SolidColorBrush* getPtr();										// Returns the d2d brush pointer as a solid color brush
		HRESULT create(ID2D1HwndRenderTarget*) override;					// Gets a value for the pointer
	};
	class linearBrush : brush
	{
		ID2D1LinearGradientBrush* pLinearBrush;
	public:

	};
	class radialBrush : brush
	{
		ID2D1RadialGradientBrush* pRadialBrush;
	public:

	};
	class bitmapBrush : brush
	{
		ID2D1BitmapBrush* pBitmapBrush;
	public:

	};

	static class DEFS	// Holds default values for various things, as well as the prefered container
	{

	public:
		container* container;

	}DEFAULTS;

	// Container
	class container
	{
		dynArray<object*> m_objs;
		ID2D1HwndRenderTarget* m_pRT;
	public:
		// tors
		container();									

		// Methods
		void create(ID2D1HwndRenderTarget* RT);						// Gives the container the render target
		int add(object* newObject);									// Is called when an object is instantiated
		int size();													// Gets how many objects are in the container
		int operator[](object* object);								// Returns the position of an object in the queue
		object* operator[](int Index);								// Returns the object in that position of the array
		object* getObjAtCord(int X, int Y);							// returns what object is at those cordinates
		ID2D1HwndRenderTarget* getRenderTarget() { return m_pRT; };	// Gets the render target in the container
		HRESULT render();											// Renders all stored objects
		void discard();												// *To be ran in discardResources*
	};
	// Base object
	class object
	{
		// Info
		static unsigned sm_count;		// Increment every time an object is made
		const int m_id;					// Each object gets and id when created
		objectType m_type;				// What type the object is
		object* m_parent;				// The parent of the object
		dynArray<object*> m_children;	// The children of the object
		bool m_enebled = 1;				// Is the object enabled

		// Brushes
		brushType m_useBrush = brushType::solid;
		brush* m_brush = nullptr;

	protected:
		object(objectType Type = objectType::rect);	// Init with object type
		
	public:
		// brush stuff
		brushType getBrushType() { return m_useBrush; };
		brush* getBrush();

		// Virtuals
		virtual void move(float X, float Y) = 0;			// Moves the object on the screen relative to its current position
		virtual void setPos(float X, float Y) = 0;			// Moves the object to a position relative to the window
		virtual bool inBounds(int X, int Y) { return 0; };	// Returns if the object is in a position
		virtual HRESULT render(ID2D1HwndRenderTarget* pRenderTarget) = 0;	// puts the object on the screen
		virtual HRESULT EventFunction(MKEvent) { return S_OK; };				// Runs the Event system function
	};
	unsigned object::sm_count = 0;

	class rect : public object
	{
		// Visuals
		D2D1_RECT_F m_rect;		// The visual size of the rectangle
		bool m_fill = 1;	// Is the rectangle filled
		bool m_rounded = 0;	// Is the rectangle rounded
		float m_weight = 2; // When not filled the thickness of the rectangle

		// Interaction
		RECT m_bounds;		// The interaction size of the rectangle
		bool m_lockedBounds = 1; // are the bounds locked to the rect
		RECT m_restriction;	// Where the objects position is restricted to
		float m_originX;
		float m_originY;	// Origin is where the object will scale from
		float m_scaleX = 1;
		float m_scaleY = 1;	// Scale changes the objects size without writing to its size
		float m_rotation = 0;

	public:
		rect(float X = 75, float Y = 75, float W = 100, float H = 100);

		void move(float X, float Y) override;
		void setPos(float X, float Y) override;
		bool inBounds(int X, int Y) override;
		HRESULT render(ID2D1HwndRenderTarget* pRenderTarget) override;

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