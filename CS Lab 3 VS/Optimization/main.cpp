#include <fstream>
#include <random>

using namespace std;
constexpr int LINE_COUNT = 1000, POINTS_COUNT = 3000000;
double y[POINTS_COUNT], x[POINTS_COUNT];
float b[LINE_COUNT], k[LINE_COUNT];

int main(void)
{
	std::ifstream ifstr("input.txt");
	for (int i = 0; i < POINTS_COUNT; ++i)
		ifstr >> x[i];
	for (int i = 0; i < LINE_COUNT; ++i)
		ifstr >> b[i] >> k[i];
	ifstr.close();
	for (int j = 0; j < POINTS_COUNT; ++j)
		for (int i = 0; i < LINE_COUNT; ++i)
			y[j] += b[i] + k[i] * x[j];
	std::ofstream ofstr("output.txt");
	for (int j = 0; j < POINTS_COUNT; ++j)
		ofstr << y[j] << '\n';
	return 0;
}