#include "Tetris.hpp"
#include "Imgui/imgui.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace Tetris {

	constexpr char NULL_LABLE[] = "  ";
	const ImVec4 EmptyCellColor = (ImVec4)ImColor(1.0f, 1.0f, 1.0f, 0.2f);
	const ImVec4 FilledCellColor = (ImVec4)ImColor::HSV(0.1f, 0.6f, 0.6f);

	bool Block::Contain(uint y, uint x)
	{
		for (auto& c : Cells) {
			auto ny = OffsetY + c.y;
			auto nx = OffsetX + c.x;
			if (ny == y && nx == x) return true;
		}
		return false;
	}
	void Block::Clear()
	{
		Cells.clear();
		OffsetY = 0;
		OffsetX = 0;
	}

	Game::Game() : m_Board(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false))
	{
	}
	Game::~Game()
	{
	}
	void Game::Update()
	{
		//Update on every 1 second
		
		if (m_IsGameOver) return;

		if (m_CurrentFallingBlock.Cells.empty()) {
			if (!CreateFallingBlock()) {
				m_IsGameOver = true;
				m_CurrentFallingBlock.Clear();
			}
			return;
		}

		if (!MoveBlock(1, 0)) {
			for (auto& c : m_CurrentFallingBlock.Cells) {
				auto y = m_CurrentFallingBlock.OffsetY + c.y;
				auto x = m_CurrentFallingBlock.OffsetX + c.x;
				m_Board[y][x] = true;
			}
			ClearFullLineAndCompact();
			m_CurrentFallingBlock.Clear();
		}
	}
	void Game::ClearFullLineAndCompact()
	{
		for (uint y = 0; y < BOARD_HEIGHT; y++) {
			if (!all_of(m_Board[y].begin(), m_Board[y].end(), [](bool isEmpty) {return isEmpty; }))
				continue;
			for (uint tmp = y; tmp > 0; tmp--) {
				m_Board[tmp] = m_Board[tmp - 1];
			}
		}
	}
	void Game::ResetGame()
	{
		for (auto& line : m_Board) {
			fill(line.begin(), line.end(), false);
		}
		m_IsGameOver = false;
	}
	void Game::OnInput(KeyInput input)
	{
		if (m_IsGameOver) return;

		switch (input) {
		case KeyInput::Left: MoveBlock(0, -1); cout << "Left\n";  break;
		case KeyInput::Right: MoveBlock(0, 1);  cout << "Right\n"; break;
		case KeyInput::Down: MoveBlock(1, 0);  cout << "Down\n"; break;
		case KeyInput::Up: Rotate(); cout << "Up\n"; break;
		}
	}
	void Game::OnDraw()
	{
		ImGui::Text("Current Falling Block Offest y : %u, x : %u", m_CurrentFallingBlock.OffsetY, m_CurrentFallingBlock.OffsetX);
		for (auto& c : m_CurrentFallingBlock.Cells) {
			ImGui::Text("( %u, %u ) ", c.y, c.x);
			ImGui::SameLine();
		}
		ImGui::Text(" ");

		if (m_IsGameOver && ImGui::Button("GameOver!!!     Reset??")) {
			ResetGame();
		}

		for (uint y = 0; y < BOARD_HEIGHT; y++) {
			for (uint x = 0; x < BOARD_WIDTH; x++) {
				auto cellColor = (m_Board[y][x] | m_CurrentFallingBlock.Contain(y, x)) ? FilledCellColor : EmptyCellColor;

				ImGui::PushID(y * BOARD_WIDTH + x);
				ImGui::PushStyleColor(ImGuiCol_Button, cellColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, cellColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, cellColor);
				ImGui::Button(NULL_LABLE, ImVec2(30, 30));
				ImGui::PopStyleColor(3);
				ImGui::PopID();

				if (x != BOARD_WIDTH - 1) ImGui::SameLine();
			}
		}

	}
	bool Game::CreateFallingBlock()
	{
		m_CurrentFallingBlock.OffsetX = 0;
		m_CurrentFallingBlock.OffsetY = 0;

		uint index = rand() % 7;
		//uint index = 0;
		switch (index)
		{
		case 0: m_CurrentFallingBlock.Cells = { {0, 0}, {0, 1}, { 1, 0}, {1, 1} }; m_CurrentFallingBlock.RotateSquareSize = 2; break; // O
		case 1: m_CurrentFallingBlock.Cells = { {0, 1}, {1, 1}, { 2, 1}, {3, 1} }; m_CurrentFallingBlock.RotateSquareSize = 4; break; // I
		case 2: m_CurrentFallingBlock.Cells = { {0, 0}, {1, 0}, { 1, 1}, {1, 2} }; m_CurrentFallingBlock.RotateSquareSize = 3; break; // J
		case 3: m_CurrentFallingBlock.Cells = { {0, 2}, {1, 0}, { 1, 1}, {1, 2} }; m_CurrentFallingBlock.RotateSquareSize = 3; break; // L
		case 4: m_CurrentFallingBlock.Cells = { {0, 1}, {1, 0}, { 1, 1}, {1, 2} }; m_CurrentFallingBlock.RotateSquareSize = 3; break; // T
		case 5: m_CurrentFallingBlock.Cells = { {0, 0}, {0, 1}, { 1, 1}, {1, 2} }; m_CurrentFallingBlock.RotateSquareSize = 3; break; // Z
		case 6: m_CurrentFallingBlock.Cells = { {1, 0}, {1, 1}, { 0, 1}, {0, 2} }; m_CurrentFallingBlock.RotateSquareSize = 3; break; // S
		}

		for (auto& c : m_CurrentFallingBlock.Cells) {
			auto y = m_CurrentFallingBlock.OffsetY + c.y;
			auto x = m_CurrentFallingBlock.OffsetX + c.x;
			if (!IsValidPos(y, x)) return false;
		}
		return true;
	}
	bool Game::IsValidPos(uint y, uint x)
	{
		return y < BOARD_HEIGHT && x < BOARD_WIDTH && m_Board[y][x] == false;
	}
	bool Game::MoveBlock(int moveY, int moveX)
	{
		for (auto& c : m_CurrentFallingBlock.Cells) {
			auto ny = m_CurrentFallingBlock.OffsetY + c.y + moveY;
			auto nx = m_CurrentFallingBlock.OffsetX + c.x + moveX;
			if (!IsValidPos(ny, nx)) return false;
		}

		m_CurrentFallingBlock.OffsetY += moveY;
		m_CurrentFallingBlock.OffsetX += moveX;
		return true;
	}
	
	bool Game::Rotate()
	{
		for (auto& c : m_CurrentFallingBlock.Cells) {
			auto ny = m_CurrentFallingBlock.OffsetY + c.x;
			auto nx = m_CurrentFallingBlock.OffsetX + m_CurrentFallingBlock.RotateSquareSize - c.y - 1;
			if (!IsValidPos(ny, nx)) return false;
		}
		for (auto& c : m_CurrentFallingBlock.Cells) {
			auto y = c.x;
			auto x = m_CurrentFallingBlock.RotateSquareSize - c.y - 1;
			c.y = y;
			c.x = x;
		}
		return true;
	}
}
