#include "GameMap.hpp"

static int cols = 1280 / CELL_SIZE;
static int lastLine = 720 / CELL_SIZE - 1;

GameMap::GameMap()
{
	for (int i = 0; i < 1280 / C::GRID_SIZE; ++i)
		m_cells.push_back({ CellType::Wall, sf::Vector2i(i, lastLine) });

	m_cells.push_back({ CellType::Wall, sf::Vector2i(0, lastLine - 1)});
	m_cells.push_back({ CellType::Wall, sf::Vector2i(0, lastLine - 2)});
	m_cells.push_back({ CellType::Wall, sf::Vector2i(0, lastLine - 3)});

	m_cells.push_back({ CellType::Wall, sf::Vector2i(cols - 1, lastLine - 1)});
	m_cells.push_back({ CellType::Wall, sf::Vector2i(cols - 1, lastLine - 2)});
	m_cells.push_back({ CellType::Wall, sf::Vector2i(cols - 1, lastLine - 3)});

	m_cells.push_back({ CellType::Wall, sf::Vector2i(cols >> 2, lastLine - 2)});
	m_cells.push_back({ CellType::Wall, sf::Vector2i(cols >> 2, lastLine - 3)});
	m_cells.push_back({ CellType::Wall, sf::Vector2i(cols >> 2, lastLine - 4)});
	m_cells.push_back({ CellType::Wall, sf::Vector2i((cols >> 2) + 1, lastLine - 4) });

	wallSprites.clear();
	for (Cell& cell : m_cells)
	{
		if (cell.type == CellType::Wall)
		{
			sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
			rect.setPosition(cell.coo.x * CELL_SIZE, cell.coo.y * CELL_SIZE);
			rect.setFillColor(sf::Color(0x07ff07ff));
			wallSprites.push_back(rect);
		}
	}
}

const std::vector<Cell>& GameMap::getCells()
{
	return m_cells;
}

bool GameMap::hasCollision(sf::Vector2i coo)
{
	for(auto& cell : m_cells)
		if (cell.coo == coo and cell.type == CellType::Wall)
			return true;
	return false;
}

bool GameMap::hasCollision(int x, int y)
{
	return hasCollision({ x, y });
}

void GameMap::draw(sf::RenderWindow& win)
{
	for (auto& wall : wallSprites)
		win.draw(wall);
}
