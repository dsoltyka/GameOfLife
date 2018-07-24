#include "stdafx.h"
#include "InputManager.h"

#include "GdiRenderer.h"
#include "Simulation.h"

namespace GameOfLife
{
	InputManager::InputManager()
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::OnKeyDown(const WPARAM& aKey)
	{
		const int shiftDown = GetKeyState(VK_SHIFT) & 0x8000;
		const int64_t adjustmentValue = shiftDown ? ourMaxAdjustmentValue : ourAdjustmentValue;

		switch (aKey)
		{
			case VK_LEFT:
				{
					CellTree::Point adjustment(-adjustmentValue, 0);
					GameOfLife::GdiRenderer::GetInstance()->AdjustTopLeft(adjustment);
				}
				break;
			case VK_RIGHT:
				{
					CellTree::Point adjustment(adjustmentValue, 0);
					GameOfLife::GdiRenderer::GetInstance()->AdjustTopLeft(adjustment);
				}
				break;
			case VK_UP:
				{
					CellTree::Point adjustment(0, adjustmentValue);
					GameOfLife::GdiRenderer::GetInstance()->AdjustTopLeft(adjustment);
				}
				break;
			case VK_DOWN:
				{
					CellTree::Point adjustment(0, -adjustmentValue);
					GameOfLife::GdiRenderer::GetInstance()->AdjustTopLeft(adjustment);
				}
		}
	}

	void InputManager::OnLeftMouseDown(const int aX, const int aY)
	{
		const CellTree::Point& topLeft = GameOfLife::GdiRenderer::GetInstance()->GetTopLeft();
		const uint16_t pixelSize = GameOfLife::GdiRenderer::GetInstance()->GetPixelSize();

		const CellTree::Point worldPoint(topLeft.myX + (aX / pixelSize), topLeft.myY - (aY / pixelSize));
		GameOfLife::Simulation::GetInstance()->CreateOscillatorAt(worldPoint);
	}

	void InputManager::OnRightMouseDown(const int aX, const int aY)
	{
		const CellTree::Point& topLeft = GameOfLife::GdiRenderer::GetInstance()->GetTopLeft();
		const uint16_t pixelSize = GameOfLife::GdiRenderer::GetInstance()->GetPixelSize();

		const CellTree::Point worldPoint(topLeft.myX + (aX / pixelSize), topLeft.myY - (aY / pixelSize));
		GameOfLife::Simulation::GetInstance()->CreateGliderGunAt(worldPoint);
	}

	void InputManager::OnMouseWheel(const short& aDelta)
	{
		if (aDelta > 1)
		{
			GameOfLife::GdiRenderer::GetInstance()->AdjustPixelSize(1);
		}
		else if (aDelta < 1)
		{
			GameOfLife::GdiRenderer::GetInstance()->AdjustPixelSize(-1);
		}
	}
}