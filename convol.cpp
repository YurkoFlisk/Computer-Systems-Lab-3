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
			output[i] += kernel[k + kernelN] * input[max(min(i - k, inputSize - 1), 0)]; // i - k >= 0
	}
}

constexpr int KERNEL_N = 200, SEQUENCE_SIZE = 5000000,
	PADDING = 1000; // To avoid overflow in convolution function
float input[SEQUENCE_SIZE + PADDING] = {}, kernel[2 * KERNEL_N + 1], output[SEQUENCE_SIZE];

int main()
{
	int seed;
	cin >> seed;
	uniform_real_distribution<> dist(-500, 500);
	minstd_rand gen(seed);
	for (int i = -KERNEL_N; i <= KERNEL_N; ++i)
		kernel[i + KERNEL_N] = dist(gen);
	for (int i = 0; i < SEQUENCE_SIZE; ++i)
		input[i] = dist(gen);
	convolution(input, SEQUENCE_SIZE, kernel, KERNEL_N, output);
	// convolution2(input, SEQUENCE_SIZE, kernel, KERNEL_N, output);
	const float sum = accumulate(output, output + SEQUENCE_SIZE, 0, [](float sum, float cur) {
		return sum + cur;
	});
	cout << sum / SEQUENCE_SIZE << endl;
	return 0;
}