
#include <thread>
#include <mutex>
#include <vector>
#include "key_search_helpers.h"
#include "key_search.h"

// Max keys to search for,
unsigned int g_max;

// How many keys we have found so far.
unsigned int g_count;

// Mutex guard for g_count.
std::mutex g_count_mtx;

// Thread process.
static void thr_proc(const strlist_t& word_list) {

	struct ec_keypair pair;

	while (g_count < g_max) {
		std::string word;

		if (key_search(&pair, word, word_list)) {

			// Guard output with mutex, so we don't get
			// interrupted mid write and can write to g_count safely.
			const std::lock_guard<std::mutex> lock(g_count_mtx);

			// It is possible g_count was updated by another thread
			// after we checked it in the while loop.
			// So while we have the lock, we need to check it again.
			if (g_count >= g_max) {
				return;
			}

			// Update count and print result.
			g_count++;
			key_search_result(word, &pair);
		}
	}
}

void key_search_nt(const strlist_t& word_list, size_t n, size_t n_threads) {

	std::vector<std::thread> t;

	// Not enough threads passed in by caller.
	if (n_threads < 2) {
		// Just use linear function.
		key_search_n(word_list, n);
		return;
	}

	t.resize(n_threads - 1);

	// Setup global variables for the threads.
	g_max = n;
	g_count = 0;

	// Launch them.
	for(int i = 0; i < t.size(); i++) {
		t[i] = std::thread(thr_proc, word_list);
	}

	// Use main thread for 1 search
	thr_proc(word_list);

	// Wait for all threads to compelete.
	for(int i = 0; i < t.size(); i++) {
		t[i].join();
	}
}
