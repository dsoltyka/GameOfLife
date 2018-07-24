#pragma once

#include <stdint.h>

namespace GameOfLife
{
	class InputManager
	{
	public:
		static InputManager* GetInstance()
		{
			static InputManager* theInstance = nullptr;
			if (!theInstance)
				theInstance = new InputManager();

			return theInstance;
		}

		~InputManager();

		void OnKeyDown(const WPARAM& aKey);

		void OnLeftMouseDown(const int aX, const int aY);
		void OnRightMouseDown(const int aX, const int aY);
		void OnMouseWheel(const short& aDelta);

	private:

		InputManager();

		static const int64_t ourAdjustmentValue = 50;
		static const int64_t ourMaxAdjustmentValue = 0x7fffffffffffffff;
	};
}
