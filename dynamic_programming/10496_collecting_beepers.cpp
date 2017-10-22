/*********************************************************************
Collecting beepers
------------------
UVa ID: 10496

Goal:	find the minimum distance that a 2D robot has to move to get
		from its starting position to each of the beepers and
		back to the starting position.

This file is covered by the LICENSE file in the root of this project.
**********************************************************************/

#include "base.hpp"
#include "dp_tsp.hpp"
#include <cstddef>
#include <vector>
#include <iostream>
#include <cassert>

class Collecting_beepers : public CP1
{
private:
	struct Cell
	{
		unsigned int x;
		unsigned int y;
	};

private:
	virtual void read_input() override
	{
		// <number of test cases>
		// <size_x> <size_y>
		// <start_x> <start_y>
		// <number of beepers>
		// <beeper_1_x> <beeper_1_y>
		// ...
		// <beeper_n_x> <beeper_n_y>

		unsigned int ignore_size_x, ignore_size_y;
		std::cin >> ignore_size_x >> ignore_size_y >> start.x >> start.y;

		std::size_t n;
		std::cin >> n;

		assert(n <= 10);

		cells.resize(n);
		for (auto& p : cells)
			std::cin >> p.x >> p.y;
	}

	virtual void solve(std::size_t) override
	{
		cells.push_back(start);

		const auto shortest_cycle = shortest_hamiltonian_cycle_weight(cells.size(),
			[this](std::size_t i, std::size_t j)
		{
			const auto& ci = cells[i];
			const auto& cj = cells[j];
			const auto dx = (ci.x > cj.x) ? ci.x - cj.x : cj.x - ci.x;
			const auto dy = (ci.y > cj.y) ? ci.y - cj.y : cj.y - ci.y;
			return dx + dy;
		});

		std::cout << "The shortest path has length " << shortest_cycle << '\n';
	}

private:
	Cell start;
	std::vector<Cell> cells;
};

int main()
{
	Collecting_beepers p;
	return p.run();
}