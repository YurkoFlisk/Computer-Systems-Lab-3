#include <iostream>
#include <random>
#include <numeric>

using namespace std;

// Computes convoultion of 'input' sequence and 'kernel' (as if it was indexed
// from -'kernelN' to +'kernelN' inclusive) and writes that to 'output'
// Kernel size is 2*'kernelN' + 1, input (and output) size is 'inputSize'
void convolution(float input[], int inputSize, float kernel[], int kernelN, float output[])
{
	for (int i = 0; i < inputSize; ++i)
	{
		output[i] = 0;
		for (int k = -kernelN; k <= kernelN; ++k)
			output[i] += kernel[k + kernelN] * input[max(min(i - k, inputSize - 1), 0)];
	}
}

constexpr int KERNEL_N = 200, MAX_SEQUENCE_SIZE = 5000000;
	// ,PADDING = 1000; // To avoid overflow in convolution function
float input[MAX_SEQUENCE_SIZE], kernel[2 * KERNEL_N + 1], output[MAX_SEQUENCE_SIZE];

int main()
{
	int n, seed;
	cin >> n >> seed;
	if (n < 1)
		n = 1;
	if (n > MAX_SEQUENCE_SIZE)
		n = MAX_SEQUENCE_SIZE;
	uniform_real_distribution<> dist(-500, 500);
	minstd_rand gen(seed);
	for (int i = -KERNEL_N; i <= KERNEL_N; ++i)
		kernel[i + KERNEL_N] = dist(gen);
	for (int i = 0; i < n; ++i)
		input[i] = dist(gen);
	convolution(input, n, kernel, KERNEL_N, output);
	const float sum = accumulate(output, output + n, 0, [](float sum, float cur) {
		return sum + cur;
	});
	cout << static_cast<int>(sum / n) << endl;
	return 0;
}