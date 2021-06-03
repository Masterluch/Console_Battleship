#ifndef SHIP_HPP
#define SHIP_HPP

class Ship
{
private:
	size_t _coordinate_num;
	size_t _coordinate_letter;
	bool _direction;
	size_t _ship_size;
	std::vector<bool> _cells;

public:
	Ship(size_t coordinate_num, size_t coordinate_letter, bool direction, size_t ship_size) : _coordinate_num(coordinate_num), _coordinate_letter(coordinate_letter), _direction(direction), _ship_size(ship_size)
	{
		for (size_t i = 0; i < _ship_size; ++i)
		{
			_cells.push_back(1);
		}
	}

	size_t get_num() const
	{
		return _coordinate_num;
	}

	size_t get_letter() const
	{
		return _coordinate_letter;
	}

	size_t get_ship_size() const
	{
		return _ship_size;
	}
};

#endif //!SHIP_HPP