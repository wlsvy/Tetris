#pragma once
#include <vector>

namespace Tetris {
	using uint = unsigned int;
	constexpr uint BOARD_WIDTH = 10;
	constexpr uint BOARD_HEIGHT = 17;

	enum class KeyInput : char {
		Up,
		Down,
		Left,
		Right
	};

	class Game {
	public:
		void OnUpdate();
		void OnInput(KeyInput input);
		void OnDraw();

	private:
		void ClearFullLineAndCompact();
		void ResetGame();
		bool IsValidPos(uint y, uint x);
		bool MoveBlock(int y, int x);
		bool RotateBlock();
		bool CreateFallingBlock();

		struct Cell {
			uint y = 0, x = 0;
		};
		struct Block {
			bool IsOccupied(uint y, uint x);
			void Clear();

			std::vector<Cell> Cells;
			uint OffsetX = 0;
			uint OffsetY = 0;
			uint RotateSquareSize;
		};

		bool m_IsGameOver = false;
		std::vector<std::vector<bool>> m_Board = std::vector<std::vector<bool>>(BOARD_HEIGHT, std::vector<bool>(BOARD_WIDTH, false));
		Block m_CurrentFallingBlock;
	};
}