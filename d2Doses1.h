#pragma once
#include "LibDosesWin.h"

namespace d2Doses1
{
	static class DEFS	// Holds default values for various things, as well as the prefered container
	{

	public:
		container* container;
	}DEFAULTS;

	class container
	{
		dynArray<object*> objs;
		ID2D1HwndRenderTarget* pRT;
	public:
		// tors
		container() : pRT(NULL) {};

		void create(ID2D1HwndRenderTarget* RT);

		int add(object* newObject);

	};

	class object
	{
		object() {};
	public:

	};

	class rect : public object
	{

	};
}

#include "d2Doses1.cpp"