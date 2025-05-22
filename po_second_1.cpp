#include <iostream>
#include <vector>
#include <climits>
#include <chrono>

int main() {
	const int n = 1000000;
	std::vector<int> arr(n);
	for (int i = 0; i < n; ++i) {
    	arr[i] = rand();
	}

	int even_count = 0;
	int max_even = INT_MIN;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < n; ++i) {
    	if (arr[i] % 2 == 0) {
        	even_count++;
        	if (arr[i] > max_even) {
            	max_even = arr[i];
        	}
    	}
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;

	std::cout << "Number of even elements: " << even_count << std::endl;
	if (even_count > 0)
    	std::cout << "Maximum even element: " << max_even << std::endl;
	else
    	std::cout << "No even elements." << std::endl;
	std::cout << "Execution time: " << diff.count() << " sec." << std::endl;

	return 0;
}
