#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Vei2.h"
#include "MainWindow.h"
class Painter
{
public:
	Painter() = default;
	Painter(Graphics& gfx, MainWindow& wnd)
		:
		gfx(gfx),
		wnd(wnd)
	{

	}
	~Painter()
	{
		delete[] _pixelPosAry;
	}

	void Update(float const deltatime)
	{		
		Init(_isInit);
		GUI();
		GetGUIDate();
		if (!_isTemporaryDraw)
		{
			UpdataPixelPosAry(deltatime);
			PosAryGetData();
		}
		
		
		if (_isGetData && Bound(*_pixelPosAry))
		{			
			Paint();			
		}
	}

	const int GetMaxPixel()const
	{
		return gfx.ScreenWidth * gfx.ScreenHeight;
	}
	void Init(bool& isInit)
	{
		if (!isInit)
		{
			isInit = true;
			_pixelPosAry = new Vei2[_canvasSize.x * _canvasSize.y];
		}		
	}

	void GraphDebug()
	{
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50 ; y++)
			{
				gfx.PutPixel(x, y, Colors::White);
			}
		}
	}
	void UpdataPixelPosAry(float const deltatime)
	{
		if (_isMoveGUI)
		{
			_pixelPosAry = new Vei2[_canvasSize.x * _canvasSize.y];
			GraphDebug();
		}
	}

	void BaseDraw()
	{
		MainDraw(_canvasSize, _pixelPosAry);
	}
	void SetCanvasSize(Vei2& canvasSize)
	{		
		if (canvasSize.x < 0)
		{
			_canvasSize.x = 0;
		}
		else if (canvasSize.x > gfx.ScreenWidth)
		{
			_canvasSize.x = gfx.ScreenWidth;
		}
		else
		{
			_canvasSize.x = canvasSize.x;
		}

		if (canvasSize.y < 0)
		{
			_canvasSize.y = 0;
		}
		else if (canvasSize.y > gfx.ScreenHeight)
		{
			_canvasSize.y = gfx.ScreenWidth;
		}
		else
		{
			_canvasSize.y = canvasSize.y;
		}
	}

	const Vei2 GetCanvasSize()
	{
	 	return _canvasSize;
	}
private:
	void DrawBox(Vei2 size, Vei2 pos)
	{
		Vei2 MaxPos = pos + size;
		Vei2 MinPos = pos;
		if (Bound(MaxPos) && Bound(MinPos))
		{
			for (int x = 0; x < size.x; x++)
			{
				for (int y = 0; y < size.y; y++)
				{

					gfx.PutPixel(x + pos.x, y + pos.y, Colors::White);

				}
			}
		}		
	}
	bool GUIinterface_Click(Vei2 size, Vei2 const pos)const
	{
		Vei2 mousePos = Vei2(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());

		if (mousePos.x >= pos.x && mousePos.y >= pos.y && mousePos.x <= pos.x + size.x && mousePos.y <= pos.y + size.y)
		{
			if (wnd.mouse.LeftIsPressed())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	void MoveGUI(Vei2 size,Vei2& pos)
	{
		if (GUIinterface_Click(size, pos))
		{
			_isMoveGUI = true;						
		}
		if (_isMoveGUI)
		{
			if (wnd.mouse.LeftIsPressed())
			{
				pos.x = wnd.mouse.GetPosX();
			}
			else
			{
				_isMoveGUI = false;
			}
				
		}
		
	}
	void GUI()
	{
		DrawBox(Vei2(12, 30), _slide);
		MoveGUI(Vei2(12, 30), _slide);
	}
	void GetGUIDate()
	{
		int slideValue;
		slideValue = _slide.x - _slideInit.x;				
		SetCanvasSize(Vei2(_canvasSizeInit.x + slideValue * 2, _canvasSizeInit.y + slideValue));
	}
	void PosAryGetData()
	{
		GetPixelPos(_canvasSize, _pixelPosAry);
	}
	void GetPixelPos(Vei2 canvasSize, Vei2* pixelPosAry)
	{
		int startPos_X = (gfx.ScreenWidth - canvasSize.x) / 2;
		int endPos_X = startPos_X + canvasSize.x;

		int startPos_Y = (gfx.ScreenHeight - canvasSize.y) / 2;
		int endPos_Y = startPos_Y + canvasSize.y;

		for (int x = startPos_X; x < endPos_X; x++)
		{
			for (int y = startPos_Y; y < endPos_Y; y++, pixelPosAry++)
			{
				pixelPosAry->y = y;
				pixelPosAry->x = x;
			}
		}
	}
	bool Bound(Vei2 screenPos)
	{
		if (screenPos.x >= 0 && screenPos.x <= gfx.ScreenWidth)
		{
			if (screenPos.y >= 0 && screenPos.y <= gfx.ScreenHeight)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	void Paint()
	{

	}
	void MainDraw(Vei2 canvasSize,Vei2* pixelPosAry)
	{
		if (_isTemporaryDraw)
		{
			int startPos_X = (gfx.ScreenWidth - canvasSize.x) / 2;
			int endPos_X = startPos_X + canvasSize.x;

			int startPos_Y = (gfx.ScreenHeight - canvasSize.y) / 2;
			int endPos_Y = startPos_Y + canvasSize.y;

			for (int x = startPos_X; x < endPos_X; x++)
			{
				for (int y = startPos_Y; y < endPos_Y; y++, pixelPosAry++)
				{
					gfx.PutPixel(x,y,Colors::White);
				}
			}
		}
		else
		{
			int size = canvasSize.x * canvasSize.y;
			Vei2* pixelPosAryEnd = pixelPosAry + size;
			while (pixelPosAry < pixelPosAryEnd)
			{
				if (Bound(Vei2(pixelPosAry->x, pixelPosAry->y)))
				{
					gfx.PutPixel(pixelPosAry->x, pixelPosAry->y, Colors::White);
				}
				pixelPosAry++;
			}
		}	
	}
private:
	bool _isInit = false;
	bool _isGetData = true;
	bool _isMoveGUI = false;
	bool _isTemporaryDraw = true;
	Vei2 _canvasSize = Vei2(400,250);
	const Vei2 _canvasSizeInit = _canvasSize;
	Vei2* _pixelPosAry;
	Graphics& gfx;
	MainWindow& wnd;
	Vei2 _slide = Vei2(400, 20);
	const Vei2 _slideInit = _slide;
};

