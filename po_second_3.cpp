#include <iostream>
#include <vector>
#include <climits>
#include <thread>
#include <atomic>
#include <chrono>

void process_chunk(const std::vector<int>& arr, int start, int end, std::atomic<int>& even_count, std::atomic<int>& max_even) {
	for (int i = start; i < end; ++i) {
    	if (arr[i] % 2 == 0) {
        	even_count.fetch_add(1, std::memory_order_relaxed);
        	int curr_val = max_even.load(std::memory_order_relaxed);
        	while (arr[i] > curr_val) {
            	if (max_even.compare_exchange_weak(curr_val, arr[i], std::memory_order_relaxed)) {
                	break;
            	}
        	}
    	}
	}
}

int main() {
	const int n = 1000000;
	const int num_threads = std::thread::hardware_concurrency();
	std::vector<int> arr(n);
	for (int i = 0; i < n; ++i) {
    	arr[i] = rand();
	}

	std::atomic<int> even_count(0);
	std::atomic<int> max_even(INT_MIN);

	auto start_time = std::chrono::high_resolution_clock::now();

	std::vector<std::thread> threads;
	int chunk_size = n / num_threads;

	for (int t = 0; t < num_threads; ++t) {
    	int chunk_start = t * chunk_size;
    	int chunk_end = (t == num_threads - 1) ? n : chunk_start + chunk_size;
    	threads.emplace_back(process_chunk, std::cref(arr), chunk_start, chunk_end, std::ref(even_count), std::ref(max_even));
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
