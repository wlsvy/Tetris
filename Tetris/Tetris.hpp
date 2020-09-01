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

	struct Cell {
		uint y = 0, x = 0;
	};
	struct Block {
		bool Contain(uint y, uint x);
		void Clear();

		uint OffsetX = 0;
		uint OffsetY = 0;
		uint RotateSquareSize;
		std::vector<Cell> Cells;
	};

	class Game {
	public:
		Game();
		~Game();

		void Update();
		void ClearFullLineAndCompact();
		void ResetGame();
		void OnInput(KeyInput input);
		void OnDraw();
		bool CreateFallingBlock();
		bool IsValidPos(uint y, uint x);
		bool MoveBlock(int y, int x);
		bool Rotate();

	private:
		bool m_IsGameOver = false;
		std::vector<std::vector<bool>> m_Board;
		Block m_CurrentFallingBlock;
	};
}