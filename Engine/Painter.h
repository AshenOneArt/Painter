#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Vei2.h"
#include "MainWindow.h"
#include <Windows.h>
#include <sstream>
#include <string>
class Painter
{
private:
	class Pixel
	{
	public:
		Pixel() = default;
		void SetPos(Vei2 _pos)
		{
			pos = _pos;
		}
		void SetCol(Color _col)
		{
			col = _col;
		}
		Vei2 GetPos()
		{
			return pos;
		}
		Color GetCol()
		{
			return col;
		}
	private:
		Vei2 pos = Vei2(0,0);
		Color col = Colors::White;
	};
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
		_pixelPosAry = nullptr;
		delete[] _pixelObj;
		_pixelObj = nullptr;
	}


	void Update(float const deltatime)
	{		
		Init(_isInit);
		GUI();
		GetGUIDate();
		std::stringstream strStream;
		strStream << "canvasSize.x:" << _canvasSize.x <<"\t" << "canvasSize.x:" << _canvasSize.y << std::endl;
		OutputDebugStringA(strStream.str().c_str());

		if (!_isTemporaryDraw)
		{
			UpdataPixelPosAry(deltatime);
			PosAryGetData();
			Paint(_pixelObj);
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
		if (!_isHeapAryGer)
		{
			_pixelPosAry = new Vei2[GetCanvasArySize()];
			_pixelObj = new Pixel[GetCanvasArySize()];
			GraphDebug();
			_isHeapAryGer = true;
		}
		
	}

	void BaseDraw()
	{
		MainDraw(_canvasSize, _pixelObj);
	}
	void MainDraw(Vei2 canvasSize, Pixel* pixelObj)
	{
		if (_isTemporaryDraw)
		{
			int startPos_X = (gfx.ScreenWidth - canvasSize.x) / 2;
			int endPos_X = startPos_X + canvasSize.x;

			int startPos_Y = (gfx.ScreenHeight - canvasSize.y) / 2;
			int endPos_Y = startPos_Y + canvasSize.y;

			for (int x = startPos_X; x < endPos_X; x++)
			{
				for (int y = startPos_Y; y < endPos_Y; y++)
				{
					gfx.PutPixel(x, y, Colors::White);
				}
			}		
		}
		else if(_isHeapAryInit)
		{
			Pixel* pixelPosAryEnd = pixelObj + GetCanvasArySize();
			while (pixelObj < pixelPosAryEnd)
			{
				gfx.PutPixel(pixelObj->GetPos().x, pixelObj->GetPos().y, pixelObj->GetCol());
				pixelObj++;
			}
		}
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
	int GetCanvasArySize()
	{
		return _canvasSize.x * _canvasSize.y;
	}
	const Vei2 GetCanvasSize()
	{
	 	return _canvasSize;
	}
private:
	void DrawBox(Vei2 size, Vei2 pos,Color col)
	{
		Vei2 MaxPos = pos + size;
		Vei2 MinPos = pos;
		if (Bound(MaxPos) && Bound(MinPos))
		{
			for (int x = 0; x < size.x; x++)
			{
				for (int y = 0; y < size.y; y++)
				{

					gfx.PutPixel(x + pos.x, y + pos.y, col);

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
				pos.x = std::max(300, pos.x);
				pos.x = std::min(600, pos.x);
			}
			else
			{
				_isMoveGUI = false;
			}
				
		}
		
	}
	void GUI()
	{
		if (_isDrawSizeGUI)
		{
			DrawBox(Vei2(12, 30), _slide, Colors::White);
			MoveGUI(Vei2(12, 30), _slide);
			DrawBox(Vei2(60, 25), Vei2(735, 5), Color(200, 50, 50));
			DrawBox(Vei2(60, 25), Vei2(735, 35), Color(50, 200, 50));
		}				
	}
	void GetGUIDate()
	{
		int slideValue;
		slideValue = _slide.x - _slideInit.x;				
		SetCanvasSize(Vei2(_canvasSizeInit.x + slideValue * 1.5, _canvasSizeInit.y + slideValue));
		
		/**********  Click Red Button  *************/
		if (GUIinterface_Click(Vei2(60, 25), Vei2(735, 5)))
		{
			_slide.x = _slideInit.x;
		}
		/**********  Click Green Button  *************/
		if (GUIinterface_Click(Vei2(60, 25), Vei2(735, 35)))
		{
			_isDrawSizeGUI = false;
		}
		/**********  Gernaerate Canvas Array On Heap Bool  *************/
		if (!_isDrawSizeGUI)
		{
			_isTemporaryDraw = false;
		}

	}
	void PosAryGetData()
	{
		if (_isHeapAryGer)
		{
			GetPixelPos(_canvasSize, _pixelPosAry);			
			SetPixelObj(_canvasSize, _pixelPosAry, _pixelObj);
		}		
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
		_isHeapAryInit = true;
	}
	void SetPixelObj(Vei2 canvasSize, Vei2* pixelPosAry,Pixel* pixelObj)
	{
		Vei2* pixelPosAryEnd = pixelPosAry + GetCanvasArySize();		
		while (pixelPosAry < pixelPosAryEnd)
		{
			pixelObj->SetPos(*pixelPosAry);
			pixelObj++;
			pixelPosAry++;
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
	void Paint(Pixel* pixelObj)
	{
		Pixel* pixelPosAryEnd = pixelObj + GetCanvasArySize();
		Vei2 mousePos;
		mousePos.x = wnd.mouse.GetPosX();
		mousePos.y = wnd.mouse.GetPosY();
		while (pixelObj < pixelPosAryEnd)
		{
			if (wnd.mouse.LeftIsPressed())
			{
				if (pixelObj->GetPos().x == mousePos.x && pixelObj->GetPos().y == mousePos.y)
				{
					pixelObj->SetCol(Colors::Black);
				}
				//pixelObj->SetCol(Colors::Cyan);
			}
			
			pixelObj++;
		}
	}
	
private:
	bool _isInit = false;
	bool _isMoveGUI = false;
	bool _isTemporaryDraw = true;
	bool _isDrawSizeGUI = true;
	bool _isHeapAryGer = false;
	bool _isHeapAryInit = false;
	Vei2 _canvasSize = Vei2(400,250);
	const Vei2 _canvasSizeInit = _canvasSize;
	Vei2* _pixelPosAry = nullptr;
	Graphics& gfx;
	MainWindow& wnd;
	Vei2 _slide = Vei2(400, 20);
	const Vei2 _slideInit = _slide;

	Pixel* _pixelObj = nullptr;
};

