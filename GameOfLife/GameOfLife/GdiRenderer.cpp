#include "stdafx.h"
#include "GdiRenderer.h"

#include "Simulation.h"

#include "../CellTree/Point.h"
#include "../CellTree/Tree.h"

#include <objidl.h>
#include <gdiplus.h>

namespace GameOfLife
{
	GdiRenderer::GdiRenderer()
		: myPixelSize(1U)
	{
	}

	GdiRenderer::~GdiRenderer()
	{
	}

	void GdiRenderer::Render(HDC aHdc)
	{
		Gdiplus::Graphics graphics(aHdc);

		const Gdiplus::Color deadColor(0xFF242424);
		if (myRequiresClear)
		{
			graphics.Clear(deadColor);
			Simulation::GetInstance()->GetTree()->RefreshCells();

			myRequiresClear = false;
		}

		Gdiplus::Color aliveColor = Gdiplus::Color::LightBlue;

		// Adjust blue channel based on generation for a temporal shift
		byte blue = aliveColor.GetBlue();
		blue += Simulation::GetInstance()->GetTree()->GetGeneration() % 255;

		aliveColor =
			Gdiplus::Color::MakeARGB(
				aliveColor.GetAlpha(),
				aliveColor.GetRed(),
				aliveColor.GetGreen(),
				blue);

		Gdiplus::SolidBrush* aliveBrush = new Gdiplus::SolidBrush(aliveColor);
		Gdiplus::SolidBrush* deadBrush = new Gdiplus::SolidBrush(deadColor);

		CellTree::Tree* tree = Simulation::GetInstance()->GetTree();

		auto iter = tree->GetUpdatedCells().begin();
		for (; iter != tree->GetUpdatedCells().end(); ++iter)
		{
			CellTree::Point point = (*iter).first;
			const bool alive = (*iter).second;

			point -= myTopLeft;
			point.myY *= -1;

			if (!IsWithinRenderBounds(point))
			{
				continue;
			}

			// Adjust red / green channels for spatial shift

			byte red = aliveColor.GetRed();
			red += (*iter).first.myX % 255;

			// Purely stylistic
			byte green = aliveColor.GetBlue();
			green += (*iter).first.myY % 255;

			aliveColor =
				Gdiplus::Color::MakeARGB(
					aliveColor.GetAlpha(),
					red,
					green,
					blue);

			aliveBrush->SetColor(aliveColor);

			const Gdiplus::Rect rect(int(point.myX) * myPixelSize, int(point.myY) * myPixelSize, myPixelSize, myPixelSize);
			const Gdiplus::SolidBrush* brush = alive ? aliveBrush : deadBrush;
			graphics.FillRectangle(brush, rect);
		}

		delete aliveBrush;
		delete deadBrush;
	}

	void GdiRenderer::SetRenderBounds(const int aWidth, const int aHeight, const bool aCenterView /*= true*/)
	{
		myRenderBounds.myX = aWidth;
		myRenderBounds.myY = aHeight;

		if (aCenterView)
		{
			SetTopLeft(CellTree::Point(-(aWidth / 2), (aHeight / 2)));
		}

		myRequiresClear = true;
	}
}