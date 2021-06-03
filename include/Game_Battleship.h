#ifndef GAME_BATTLESHIP_HPP
#define GAME_BATTLESHIP_HPP

#include "include.h"

class Game_Battleship
{
private:
	std::vector <std::vector<Game_Cell>> _player_field;
	std::vector <std::vector<Game_Cell>> _ai_field;

	std::vector<size_t> _total_num_ship;
	std::vector<size_t> _available_num_ship;

	std::vector<Ship> _player_ships;
	std::vector<Ship> _ai_ships;

	std::vector<char> _columns;

	size_t _player_ships_cell_count;
	size_t _ai_ships_cell_count;

	bool _ai_has_purpose;
	size_t _coordinate_num_ai_purpose;
	size_t _coordinate_letter_ai_purpose;

	bool _is_damage;

	template <class T>
	bool is_in_vector(T& element, const std::vector<T>& vector) const
	{
		for (auto i : vector)
		{
			if (element == i) return 1;
		}
		return 0;
	}

	std::vector<std::string> split_line(const std::string& split_line) const
	{
		std::vector<std::string> vector_split;
		std::string line = "";

		for (size_t i = 0; i < split_line.size(); ++i)
		{
			if ((split_line[i] == ' ') or (i == split_line.size() - 1))
			{
				if (i == split_line.size() - 1)
				{
					line += split_line[i];
				}
				vector_split.push_back(line);
				line = "";
			}
			else
			{
				line += split_line[i];
			}
		}

		return vector_split;
	}

	bool is_coordinates_vector_correct(const std::vector<std::string>& coordinates_vector) const
	{
		if ((coordinates_vector.size() == 3) and (coordinates_vector[0].size() == 1) and (coordinates_vector[1].size() == 2) and (coordinates_vector[2].size() == 1))
		{
			return 1;
		}
		else return 0;
	}

	void print_field(const std::vector<std::vector<Game_Cell>>& field) const
	{
		// print letters
		std::cout << "   ";
		for (auto i : _columns)
		{
			std::cout << i << " ";
		}
		
		std::cout << std::endl;

		// print numbers and cells
		for (size_t i = 0; i < 10; ++i)
		{
			std::cout << i << "  ";
			for (size_t j = 0; j < 10; ++j)
			{
				field[i][j].print_symbol();
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}

	void print_hidden_field(const std::vector<std::vector<Game_Cell>>& field) const
	{
		// print letters
		std::cout << "   ";
		for (auto i : _columns)
		{
			std::cout << i << " ";
		}

		std::cout << std::endl;

		for (size_t i = 0; i < 10; ++i)
		{
			std::cout << i << "  ";
			for (size_t j = 0; j < 10; ++j)
			{
				if ((field[i][j].is_defeated_cell()) and (field[i][j].is_ship()))
				{
					std::cout << "\x1B[91m" << "x" << "\033[0m";
				}
				else if (field[i][j].is_defeated_cell())
				{
					std::cout << "\x1B[91m" << "~" << "\033[0m";
				}
				else
				{
					std::cout << "~";
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}

	void clear_field(std::vector<std::vector<Game_Cell>>& field)
	{
		// clear the field
		for (size_t i = 0; i < 10; ++i)
		{
			for (size_t j = 0; j < 10; ++j)
			{
				if (field[i][j].is_ship())
				{
					field[i][j].del_ship();
				}
			}
		}
		_player_ships.clear();
	}

	void set_ship(std::vector<std::vector<Game_Cell>>& field, size_t ship_size, char coordinate_letter, size_t coordinate_num, bool direction)
	{
		if ((ship_size == 0) or (ship_size > 4) or (_available_num_ship[ship_size - 1] == 0) or (coordinate_num > 9) or (coordinate_letter - 'A' > 9) or ((direction) and (coordinate_letter - 'A' + ship_size - 1> 9)) or ((!direction) and (coordinate_num + ship_size - 1 > 9)))
		{
			throw -1;
		}

		// direction = 1 right
		if (direction)
		{
			// checking the correct coordinates
			for (size_t i = 0; i < ship_size; ++i)
			{
				if ((field[coordinate_num][i + coordinate_letter - 'A'].is_ship()) or ((coordinate_num > 0) and (field[coordinate_num - 1][i + coordinate_letter - 'A'].is_ship())) or ((coordinate_num < 9) and (field[coordinate_num + 1][i + coordinate_letter - 'A'].is_ship())))
				{
					throw - 1;
				}
			}
			// right left
			if (((coordinate_letter - 'A' > 0) and (field[coordinate_num][coordinate_letter - 'A' - 1].is_ship())) or ((coordinate_letter - 'A' < 9) and (field[coordinate_num][coordinate_letter - 'A' + 1].is_ship())))
			{
				throw - 1;
			}
			// top left
			if ((coordinate_num > 0) and (coordinate_letter - 'A' > 0) and (field[coordinate_num - 1][coordinate_letter - 'A' - 1].is_ship()))
			{
				throw - 1;
			}
			// down left
			if ((coordinate_num < 9) and (coordinate_letter - 'A' > 0) and (field[coordinate_num + 1][coordinate_letter - 'A' - 1].is_ship()))
			{
				throw - 1;
			}
			// top right
			if ((coordinate_num > 0) and (coordinate_letter - 'A' + ship_size - 1 < 9) and (field[coordinate_num - 1][coordinate_letter - 'A' + ship_size - 1 + 1].is_ship()))
			{
				throw - 1;
			}
			// down right
			if ((coordinate_num < 9) and (coordinate_letter - 'A' + ship_size - 1 < 9) and (field[coordinate_num + 1][coordinate_letter - 'A' + ship_size - 1 + 1].is_ship()))
			{
				throw - 1;
			}

			// setting ship
			for (size_t i = 0; i < ship_size; ++i)
			{
				field[coordinate_num][i + coordinate_letter - 'A'].set_ship();
			}
		}

		// direction = 0 down
		else
		{
			// checking the correct coordinates
			for (size_t i = 0; i < ship_size; ++i)
			{
				if ((field[i + coordinate_num][coordinate_letter - 'A'].is_ship()) or ((coordinate_letter - 'A' > 0) and (field[i + coordinate_num][coordinate_letter - 'A' - 1].is_ship())) or ((coordinate_letter - 'A' < 9) and (field[i + coordinate_num][coordinate_letter - 'A' + 1].is_ship())))
				{
					throw - 1;
				}
			}
			// top down
			if (((coordinate_num > 0) and (field[coordinate_num - 1][coordinate_letter - 'A'].is_ship())) or ((coordinate_num + ship_size - 1 < 9) and (field[coordinate_num + ship_size - 1 + 1][coordinate_letter - 'A'].is_ship())))
			{
				throw - 1;
			}
			// top left
			if ((coordinate_num > 0) and (coordinate_letter - 'A' > 0) and (field[coordinate_num - 1][coordinate_letter - 'A' - 1].is_ship()))
			{
				throw - 1;
			}
			// top right
			if ((coordinate_num > 0) and (coordinate_letter - 'A' < 9) and (field[coordinate_num - 1][coordinate_letter - 'A' + 1].is_ship()))
			{
				throw - 1;
			}
			// down left
			if ((coordinate_num + ship_size - 1 < 9) and (coordinate_letter - 'A' > 0) and (field[coordinate_num + ship_size - 1 + 1][coordinate_letter - 'A' - 1].is_ship()))
			{
				throw - 1;
			}
			// down right
			if ((coordinate_num + ship_size - 1 < 9) and (coordinate_letter - 'A' < 9) and (field[coordinate_num + ship_size - 1 + 1][coordinate_letter - 'A' + 1].is_ship()))
			{
				throw - 1;
			}

			// setting ship
			for (size_t i = 0; i < ship_size; ++i)
			{
				field[i + coordinate_num][coordinate_letter - 'A'].set_ship();
			}
		}

		// reducing the number of available ships
		_available_num_ship[ship_size - 1]--;
	}

	void set_ai_ships()
	{
		for (size_t i = 0; i < _total_num_ship.size(); ++i)
		{
			for (size_t j = 0; j < _total_num_ship[i]; ++j)
			{
				size_t coordinate_num = rand() % 10;
				size_t coordinate_letter = _columns[rand() % 10];
				bool direction = rand() % 2;
				set_ship(_ai_field, i + 1, coordinate_letter, coordinate_num, direction);
				//_ai_ships.push_back(Ship(coordinate_num, coordinate_letter, direction, i + 1));
			}
		}
	}

	void set_ship_area(std::vector<std::vector<Game_Cell>>& field, size_t ship_size, char coordinate_letter, size_t coordinate_num, bool direction)
	{

	}

	void player_turn(char coordinate_letter, size_t coordinate_num)
	{
		_ai_field[coordinate_num - '0'][coordinate_letter - 'A'].set_defeated_cell();
		_ai_ships_cell_count--;
	}

	void ai_turn()
	{
		if (_ai_has_purpose)
		{

		}
		else
		{
			size_t coordinate_num;
			size_t coordinate_letter;
			while (true)
			{
				coordinate_num = rand() % 10;
				coordinate_letter = rand() % 10;
				if (!_player_field[coordinate_num][coordinate_letter].is_defeated_cell())
				{
					_player_field[coordinate_num][coordinate_letter].set_defeated_cell();
					if (_player_field[coordinate_num][coordinate_letter].is_ship())
					{
						_player_ships_cell_count--;
						for (size_t i = 0; i < _player_ships.size(); ++i)
						{
							if ((_player_ships[i].get_ship_size() == 1) and (_player_ships[i].get_num() == coordinate_num) and (_player_ships[i].get_letter() == coordinate_letter))
							{
								set_ship_area(_player_field, 1, _columns[coordinate_letter], coordinate_num, 1);
							}
							else
							{
								_ai_has_purpose = 1;
								_coordinate_num_ai_purpose = coordinate_num;
								_coordinate_letter_ai_purpose = coordinate_letter;
							}
						}
					}
					break;
				}
			}
		}
	}

public:
	Game_Battleship(size_t one_deck_ships_count, size_t two_deck_ships_count, size_t three_deck_ships_count, size_t four_deck_ships_count) : _ai_has_purpose(0), _is_damage(0)
	{
		// setting the number of ships
		_total_num_ship = { one_deck_ships_count , two_deck_ships_count, three_deck_ships_count, four_deck_ships_count };
		_available_num_ship = { one_deck_ships_count , two_deck_ships_count, three_deck_ships_count, four_deck_ships_count };
		for (auto i : _total_num_ship)
		{
			_player_ships_cell_count += i;
			_ai_ships_cell_count += i;
		}

		// setting the field
		_columns = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
		for (size_t i = 0; i < 10; ++i)
		{
			_player_field.push_back(std::vector<Game_Cell>(10, Game_Cell(0)));
			_ai_field.push_back(std::vector<Game_Cell>(10, Game_Cell(0)));
		}
	}

	void start()
	{
		std::string coordinates_line;

		// setting player's ships
		while (true)
		{
			// checking the field is full
			if (!_available_num_ship[0] and !_available_num_ship[1] and !_available_num_ship[2] and !_available_num_ship[3])
			{
				break;
			}

			system("cls");
			std::cout << "   <<< BATTLESHIP >>>" << std::endl << std::endl;

			print_field(_player_field);

			std::cout << std::endl << "You have ships: " << std::endl
				<< "#### x " << _available_num_ship[3] << std::endl
				<< "###  x " << _available_num_ship[2] << std::endl
				<< "##   x " << _available_num_ship[1] << std::endl
				<< "#    x " << _available_num_ship[0] << std::endl;

			std::cout << std::endl << "(!) You need to set your ships on the field" << std::endl
				<< "Request structure: *type of the ship* *ship start coordinate* *direction [1 - right; 0 - down]*" << std::endl
				<< "Example: 1 A2 1" << std::endl << std::endl << "> ";

			getline(std::cin, coordinates_line);
			if (coordinates_line == "") continue;
			else if (coordinates_line == "cls")
			{
				clear_field(_player_field);

				// reducing the number of available ships
				for (size_t i = 0; i < 4; ++i)
				{
					_available_num_ship[i] = _total_num_ship[i];
				}
				continue;
			}
			else
			{
				std::vector<std::string> coordinates_vector = split_line(coordinates_line);
				if (is_coordinates_vector_correct(coordinates_vector))
				{
					try
					{
						set_ship(_player_field, stoi(coordinates_vector[0]), coordinates_vector[1][0], coordinates_vector[1][1] - '0', stoi(coordinates_vector[2]));
						//_player_ships.push_back(Ship(coordinates_vector[1][1] - '0', coordinates_vector[1][0] - 'A', stoi(coordinates_vector[2]), stoi(coordinates_vector[0])));
					}
					catch (...)
					{
						continue;
					}
				}
			}

			// checking the field is full
			if (!_available_num_ship[0] and !_available_num_ship[1] and !_available_num_ship[2] and !_available_num_ship[3])
			{
				break;
			}
		}

		// setting AI's ships
		while (true)
		{
			clear_field(_ai_field);

			// reducing the number of available ships
			for (size_t i = 0; i < 4; ++i)
			{
				_available_num_ship[i] = _total_num_ship[i];
			}

			//setting AI's ships
			try
			{
				set_ai_ships();
			}
			catch (...)
			{
				continue;
			}

			// checking the field is full
			if (!_available_num_ship[0] and !_available_num_ship[1] and !_available_num_ship[2] and !_available_num_ship[3])
			{
				break;
			}
		}

		// the start of game
		while (true)
		{
			// print field
			system("cls");
			print_field(_ai_field);
			print_hidden_field(_ai_field);
			print_field(_player_field);
			

			// player's turn
			std::cout << std::endl << ">";
			getline(std::cin, coordinates_line);
			if ((coordinates_line.size() == 2) and (coordinates_line[0] - 'A' <= 9) and (coordinates_line[1] - '0' <= 9))
			{
				player_turn(coordinates_line[0], coordinates_line[1]);
				ai_turn();
			}

			// cheking ships count
			if ((!_player_ships_cell_count) or (!_ai_ships_cell_count))
			{
				break;
			}
		}

		if (!_ai_ships_cell_count)
		{
			std::cout << "You've won!";
		}
		else
		{
			std::cout << "Maria've won! [AI]";
		}
	}
};

#endif //!GAME_BATTLESHIP_HPP