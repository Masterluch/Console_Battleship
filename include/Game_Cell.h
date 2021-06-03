#ifndef GAME_CELL_HPP
#define GAME_CELL_HPP

class Game_Cell
{
private:
	bool _is_ship;
	bool _is_defeated;

public:
	Game_Cell(bool is_ship) : _is_ship(is_ship), _is_defeated(0)
	{

	}

	void print_symbol() const
	{
		if (_is_ship)
		{
			if (_is_defeated)
			{
				std::cout << "\x1B[91m" << "#" << "\033[0m";
			}
			else
			{
				std::cout << "\x1B[32m" << "#" << "\033[0m";
			}
		}
		else
		{
			if (_is_defeated)
			{
				std::cout << "\x1B[91m" << "~" << "\033[0m";
			}
			else
			{
				std::cout << "~";
			}
		}
	}

	void set_ship()
	{
		if (_is_ship) return;
		_is_ship = 1;
	}

	void del_ship()
	{
		if (!_is_ship) return;
		_is_ship = 0;
	}

	void set_defeated_cell()
	{
		_is_defeated = 1;
	}

	bool is_ship() const
	{
		if (_is_ship) return 1;
		else return 0;
	}

	bool is_defeated_cell() const
	{
		return _is_defeated;
	}
};

#endif //!GAME_CELL_HPP