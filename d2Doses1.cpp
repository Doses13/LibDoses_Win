using namespace d2Doses1;
using namespace D2D1;

// Brushes
brush::brush() : m_brush(nullptr) {};
ID2D1Brush* brush::getPtr()
{
	if (!m_brush)
	{
		if (FAILED(create(DEFAULTS.container->getRenderTarget())))
		{
			ERRORS.creatingBrush = 1;
		}
	}
	return m_brush;
}
	// solid
solidBrush::solidBrush(float Red, float Green, float Blue, float Alpha)
{
	m_color = ColorF(Red, Green, Blue, Alpha);
}
solidBrush::solidBrush(unsigned int RGB, float Alpha)
{
	m_color = ColorF(RGB, Alpha);
}
ID2D1SolidColorBrush* solidBrush::getPtr()
{
	return reinterpret_cast<ID2D1SolidColorBrush*>(m_brush);
}
HRESULT solidBrush::create(ID2D1HwndRenderTarget* pRT)
{
	return pRT->CreateSolidColorBrush(m_color, reinterpret_cast<ID2D1SolidColorBrush**>(&m_brush));	// What a line this is
}
void solidBrush::setColor(D2D1_COLOR_F Color)
{
	m_color = Color;
	if (m_brush)
	{
		reinterpret_cast<ID2D1SolidColorBrush*>(m_brush)->SetColor(m_color);
	}
}
void solidBrush::setColor(unsigned int Color)
{
	m_color = ColorF(Color);
	if (m_brush)
	{
		reinterpret_cast<ID2D1SolidColorBrush*>(m_brush)->SetColor(m_color);
	}
}

// Container
container::container() : m_pRT(NULL)
{
	if (!DEFAULTS.container)
	{
		DEFAULTS.container = this;
	}
}
void container::create(ID2D1HwndRenderTarget* pRT)
{
	m_pRT = pRT;
}
int container::add(object* newObject)
{
	m_objs.push_back(newObject);
	return m_objs.size() - 1;
}
int container::size()
{
	return m_objs.size();
}
object* container::getObjAtCord(int X, int Y)
{
	object* found = nullptr;
	for (int i = size()-1; i >= 0 && !found; i--)
	{
		if (m_objs[i]->inBounds(X, Y))
		{
			found = m_objs[i];
		}
	}
	return found;
}
object* container::operator[](int Index)
{
	return m_objs[Index];
}
HRESULT container::render()
{
	for (int i = 0; i < m_objs.size(); i++)
	{
		if (FAILED(m_objs[i]->render(m_pRT)))
		{
			return S_FALSE;
		}
	}
	return S_OK;
}

// Object
object::object(objectType Type) : m_id(sm_count), m_type(Type), m_parent(nullptr), m_brush(nullptr)
{
	sm_count++;
	DEFAULTS.container->add(this);
	m_brush = new solidBrush;
}
brush* object::getBrush()
{
	return m_brush;
}
solidBrush* object::getSolidBrush()
{
	if (m_useBrush == brushType::solid)
	{
		return reinterpret_cast<solidBrush*>(m_brush);
	}
	return nullptr;
}
linearBrush* object::getLinearBrush()
{
	if (m_useBrush == brushType::linear)
	{
		return reinterpret_cast<linearBrush*>(m_brush);
	}
	return nullptr;
}
radialBrush* object::getRadialBrush()
{
	if (m_useBrush == brushType::radial)
	{
		return reinterpret_cast<radialBrush*>(m_brush);
	}
	return nullptr;
}
bitmapBrush* object::getBitmapBrush()
{
	if (m_useBrush == brushType::bitmap)
	{
		return reinterpret_cast<bitmapBrush*>(m_brush);
	}
	return nullptr;
}

// Rect
rect::rect(float X, float Y, float W, float H) : object(objectType::rect)
{
	// set bounds and actual rect
	m_bounds.top = m_rect.top = Y - (H / 2);
	m_bounds.bottom = m_rect.bottom = Y + (H / 2);
	m_bounds.left = m_rect.left = X - (W / 2);
	m_bounds.right = m_rect.right = X + (W / 2);
	// set restriction rect
	m_restriction.top = -1000000;
	m_restriction.bottom = 1000000;
	m_restriction.left - -1000000;
	m_restriction.right = 1000000;
	// set origin to center of object
	m_originX = X;
	m_originY = Y;
}
void rect::move(float X, float Y)
{
	if (m_lockedBounds)
	{
		m_bounds.top = m_rect.top += Y;
		m_bounds.bottom = m_rect.bottom += Y;
		m_bounds.left = m_rect.left += X;
		m_bounds.right = m_rect.right += X;

		m_originX += X;
		m_originY += Y;
	}
	else
	{
		//do later
	}
}
void rect::setPos(float X, float Y)
{
	if (m_lockedBounds)
	{
		m_bounds.top = m_rect.top = Y;
		m_bounds.bottom = m_rect.bottom = Y;
		m_bounds.left = m_rect.left = X;
		m_bounds.right = m_rect.right = X;

		m_originX = X;
		m_originY = Y;
	}
	else
	{
		//do later
	}
}
bool rect::inBounds(int X, int Y)
{
	float t = min(m_bounds.top, m_bounds.bottom);
	float b = max(m_bounds.top, m_bounds.bottom);
	float l = min(m_bounds.left, m_bounds.right);
	float r = max(m_bounds.left, m_bounds.right);
	return ((X <= r && X >= l) && (Y <= b && Y >= t));
}
HRESULT rect::render(ID2D1HwndRenderTarget* pRenderTarget)
{
	HRESULT hr = S_OK;

	// Get the d2d brush pointer
	ID2D1Brush* useBrush = getBrush()->getPtr();

	// Make the brush if it hasen't been linked with d2d yet;
	if (!useBrush)
	{
		if(FAILED(getBrush()->create(pRenderTarget))) return S_FALSE;
	}

	if (m_fill)
	{
		if (m_rounded)
		{
			
		}
		else // not rounded
		{
			pRenderTarget->FillRectangle(m_rect, getBrush()->getPtr());
		}
	}
	else
	{
		if (m_rounded)
		{

		}
		else
		{
			pRenderTarget->DrawRectangle(m_rect, getBrush()->getPtr(), m_weight);
		}
	}
	return hr;
}