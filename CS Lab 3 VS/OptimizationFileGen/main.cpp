#include <fstream>
#include <random>

using namespace std;
constexpr int LINE_COUNT = 1000, POINTS_COUNT = 3000000;

int main(void)
{
	uniform_real_distribution<> dist(-500, 500);
	minstd_rand gen(13);
	std::ofstream ofstr("input.txt");
	for (int j = 0; j < POINTS_COUNT; ++j)
		ofstr << dist(gen) << ' ';
	ofstr << endl;
	for (int i = 0; i < LINE_COUNT; ++i)
		ofstr << dist(gen) << ' ' << dist(gen) << endl;
	return 0;
}