// Standalone non-interactive perft for an engine
// BlendXChess

#include <iostream>
#include "engine/engine.h"

constexpr int MAX_PERFT_DEPTH = 6;
using namespace std;
using namespace BlendXChess;

int main()
{
	Game::initialize();
	Game game;
	for (int i = 1; i <= MAX_PERFT_DEPTH; ++i)
		cout << "Perft " << i << ": " << game.perft(i) << endl;
	return 0;
}