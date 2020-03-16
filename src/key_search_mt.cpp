/**
 * MIT License
 *
 * Copyright (c) 2019-2020 EOS Sw/eden
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <cstddef>
#include <thread>
#include <mutex>
#include <vector>
#include "crypto/ec.h"
#include "key_search_helpers.h"
#include "key_search.h"

namespace eoskeygen {

// Max keys to search for,
std::size_t g_max;

// How many keys we have found so far.
std::size_t g_count;

// Mutex guard for g_count.
std::mutex g_count_mtx;

// Thread process.
void KeySearch::_thr_proc() {

	struct ec_keypair pair;

	while (g_count < g_max) {
		struct key_result res;

		ec_generate_key(&pair);
		if (key_contains_word(&pair, m_words, &res)) {

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
			key_search_result(&pair, &res, m_dict);
		}
	}
}

void KeySearch::setThreadCount(size_t num)
{
	m_threads = num;
}

void KeySearch::_search_mt(size_t n)
{
	std::vector<std::thread> t;

	t.resize(m_threads - 1);

	// Setup global variables for the threads.
	g_max = n;
	g_count = 0;

	// Launch them.
	for(std::size_t i = 0; i < t.size(); i++) {
		t[i] = std::thread(&KeySearch::_thr_proc, this);
	}

	// Use main thread for 1 search
	_thr_proc();

	// Wait for all threads to compelete.
	for(std::size_t i = 0; i < t.size(); i++) {
		t[i].join();
	}
}

} // namespace eoskeygen
