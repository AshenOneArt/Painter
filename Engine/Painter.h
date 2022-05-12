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
	~Painter()
	{
		delete[] _pixelPosAry;
	}

	void Update(Graphics& gfx,float const deltatime)
	{		
		GUI();
		UpdataPixelPosAry(deltatime, gfx);
		GetData(gfx);
		
		if (_isGetData && Bound(*_pixelPosAry, gfx))
		{			
			Paint();			
		}
	}
	void Init(bool& isInit)
	{
		if (!isInit)
		{
			isInit = true;
			_pixelPosAry = new Vei2[_canvasSize.x * _canvasSize.y];
		}		
	}
	void GraphDebug(Graphics& gfx)
	{
		for (int x = 0; x < 50; x++)
		{
			for (int y = 0; y < 50 ; y++)
			{
				gfx.PutPixel(x, y, Colors::White);
			}
		}
	}
	void UpdataPixelPosAry(float const deltatime, Graphics& gfx)
	{
		Init(_isInit);
		
		float timeSave = 0;
		timeSave += deltatime;

		if (timeSave > 0.018f)
		{
			timeSave = 0;
			Dy_canvasSize = _canvasSize;
			GraphDebug(gfx);
		}
	}

	void BaseDraw(Graphics& gfx)
	{
		MainDraw(_canvasSize, _pixelPosAry,gfx);
	}
	void SetCanvasSize(Vei2& canvasSize)
	{
		_canvasSize = canvasSize;
	}
	const Vei2 GetCanvasSize()
	{
	 	return _canvasSize;
	}
private:
	void GUI()
	{

	}
	void GetData(Graphics& gfx)
	{
		GetPixelPos(_canvasSize, _pixelPosAry, gfx);
	}
	void GetPixelPos(Vei2 canvasSize, Vei2* pixelPosAry, Graphics& gfx)
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
	bool Bound(Vei2 screenPos, Graphics& gfx)
	{
		if (screenPos.x > 0 && screenPos.x < gfx.ScreenWidth)
		{
			if (screenPos.y > 0 && screenPos.y < gfx.ScreenHeight)
			{
				return true;
			}
		}
	}
	void Paint()
	{

	}
	void MainDraw(Vei2 canvasSize, Vei2* pixelPosAry, Graphics& gfx)
	{
		int size = canvasSize.x * canvasSize.y;
		Vei2* pixelPosAryEnd = pixelPosAry + size;
		while (pixelPosAry < pixelPosAryEnd)
		{
			gfx.PutPixel(pixelPosAry->x, pixelPosAry->y, Colors::White);
			pixelPosAry++;
		}
	}
private:
	bool _isInit = false;
	bool _isGetData = true;
	Vei2 _canvasSize = Vei2(1,1);
	Vei2 Dy_canvasSize;
	Vei2* _pixelPosAry;
	
};

