/*********************************************************************
Playing Boggle
--------------
UVa ID: 112 83

Boggle(R) is a classic word game played on a 4 by 4 grid of letters.
The letter grid is randomly generated by shaking 16 cubes labeled with
a distribution of letters similar to that found in English words.
Players try to find words hidden within the grid. Words are formed
from letters that adjoin horizontally, vertically, or diagonally.
However, no letter may be used more than once within a single word.
The score awarded for a word depends on its length, with longer words
being worth more points. Exact point values are shown in the table
below. A word is only ever scored once, even if it appears multiple
times in the grid.
No. of letters:	3	4	5	6	7	8 or more
Points:			1	1	2	3	5	11
In this problem, your task is to write a program that plays Boggle.
Given a letter grid and a dictionary of words, you are to calculate
the total score of all the words in the dictionary that can be found
in the grid.

Input
-----
The first line of the input file contains a number N, the number of
Boggle games that follow. Each Boggle game begins with 16 capital
letters arranged in a 4 by 4 grid, representing the board
configuration for that game. A blank line always precedes the letter
grid. Following the letter grid is a single number M (1 <= M <= 100),
the number of words in your dictionary for that game. The next M lines
contain the dictionary words, one per line, in no particular order.
Each word consists of between 3and 16 capital letters. No single word
will appear in the dictionary more than once for a given Boggle game.

Output
------
For each Boggle game in the input, your program should output the
total score for that game.

This file is covered by the LICENSE file in the root of this project.
**********************************************************************/

#include "base.hpp"
#include "static_matrix.hpp"
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <vector>

using Score = unsigned int;
using Board = Matrix<char, 4, 4>;

std::array<Position, 8> neighbours(Position pos)
{
	std::array<Position, 8> neighbours = {
		{{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}}};

	for (auto& neighbour : neighbours)
		neighbour += pos;
	return neighbours;
}

void switch_case(char& ch)
{
	assert(('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'));
	constexpr auto shift = 'a' - 'A';
	if (ch >= 'a')
		ch -= shift;
	else
		ch += shift;
}

template<class It>
bool contains_word(Board& board, Position start, It word_it, It word_last)
{
	if (word_it == word_last)
		return true;

	for (auto n : neighbours(start))
		if (is_inside_extents<Board>(n) && board(n) == *word_it)
		{
			switch_case(board(n));
			const auto found = contains_word(board, n, std::next(word_it), word_last);
			switch_case(board(n));
			if (found)
				return true;
		}

	return false;
}

bool contains_word(Board& board, const std::string& word)
{
	for (std::size_t col = 0; col < board.cols(); ++col)
		for (std::size_t row = 0; row < board.rows(); ++row)
		{
			const Position start{row, col};
			if (board(start) == word.front())
			{
				switch_case(board(start));
				const auto found = contains_word(board, start, std::next(word.begin()), word.end());
				switch_case(board(start));
				if (found)
					return true;
			}
		}
	return false;
}

Score word_score(const std::string& word)
{
	const auto length = word.length();
	return (length >= 3) + (length >= 5) + (length >= 6) + (length >= 7) * 2 + (length >= 8) * 6;
}

Score total_score(Board& board, std::vector<std::string> dictionary)
{
	Score score = 0;
	for (auto& word : dictionary)
		if (contains_word(board, word))
			score += word_score(word);
	return score;
}

class CP : public CP1
{
private:
	virtual void read_input() override
	{
		read_matrix(board_);
		read_size_vec(dictionary_);
	}

	virtual void solve(unsigned int i_case) override
	{
		write_ln("Score for Boggle game #", i_case, ": ", total_score(board_, dictionary_));
	}

private:
	Board board_;
	std::vector<std::string> dictionary_;
};

MAIN
