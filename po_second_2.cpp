#include <iostream>
#include <vector>
#include <climits>
#include <thread>
#include <mutex>
#include <chrono>

void process_chunk(const std::vector<int>& arr, int start, int end, int& even_count, int& max_even, std::mutex& mtx) {
	int local_even_count = 0;
	int local_max_even = INT_MIN;
	for (int i = start; i < end; ++i) {
    	if (arr[i] % 2 == 0) {
        	local_even_count++;
        	if (arr[i] > local_max_even) {
            	local_max_even = arr[i];
        	}
    	}
	}
	std::lock_guard<std::mutex> lock(mtx);
	even_count += local_even_count;
	if (local_max_even > max_even) {
    	max_even = local_max_even;
	}
}

int main() {
	const int n = 1000000;
	const int num_threads = std::thread::hardware_concurrency();
	std::vector<int> arr(n);
	for (int i = 0; i < n; ++i) {
    	arr[i] = rand();
	}

	int even_count = 0;
	int max_even = INT_MIN;
	std::mutex mtx;

	auto start_time = std::chrono::high_resolution_clock::now();

	std::vector<std::thread> threads;
	int chunk_size = n / num_threads;

	for (int t = 0; t < num_threads; ++t) {
    	int chunk_start = t * chunk_size;
    	int chunk_end = (t == num_threads - 1) ? n : chunk_start + chunk_size;
    	threads.emplace_back(process_chunk, std::cref(arr), chunk_start, chunk_end, std::ref(even_count), std::ref(max_even), std::ref(mtx));
	}

	for (auto& th : threads) {
    	th.join();
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end_time - start_time;

	std::cout << "Number of even elements: " << even_count << std::endl;
	if (even_count > 0)
    	std::cout << "Maximum even element: " << max_even << std::endl;
	else
    	std::cout << "No even elements." << std::endl;
	std::cout << "Execution time: " << diff.count() << " sec." << std::endl;

	return 0;
}
