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
	Painter(Graphics& gfx)
		:
		gfx(gfx)
	{

	}
	~Painter()
	{
		delete[] _pixelPosAry;
	}

	void Update(float const deltatime)
	{		
		GUI();
		UpdataPixelPosAry(deltatime);
		GetData();
		
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
		Init(_isInit);
		
		float timeSave = 0;
		timeSave += deltatime;

		if (timeSave > 0.018f)
		{
			timeSave = 0;
			_Dy_canvasSize = _canvasSize;
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
	void GUI()
	{

	}
	void GetData()
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
		if (screenPos.x > 0 && screenPos.x < gfx.ScreenWidth)
		{
			if (screenPos.y > 0 && screenPos.y < gfx.ScreenHeight)
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
	void MainDraw(Vei2 canvasSize, Vei2* pixelPosAry)
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
private:
	bool _isInit = false;
	bool _isGetData = true;
	Vei2 _canvasSize = Vei2(1,1);
	Vei2 _Dy_canvasSize;
	Vei2* _pixelPosAry;
	Graphics& gfx;
};

