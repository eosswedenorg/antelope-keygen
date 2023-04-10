/**
 * MIT License
 *
 * Copyright (c) 2019-2021 EOS Sw/eden
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
#include <chrono>
#include <libantelope/ec.hpp>
#include "benchmark.hpp"

namespace antelopekeygen {

std::chrono::duration<float> _run_benchmark(size_t num_keys) {
	auto start = std::chrono::steady_clock::now();
	for(size_t i = 0; i < num_keys; i++) {
		struct libantelope::ec_keypair k;
		libantelope::ec_generate_key(&k);
	}
	return std::chrono::steady_clock::now() - start;
}

void benchmark(size_t num_keys, struct benchmark_result* res) {

	if (num_keys < 1) {
		res->sec = res->kps = 0;
		return;
	}

	res->sec = _run_benchmark(num_keys).count();
	res->kps = static_cast<float>(num_keys) / res->sec;
}

} // namespace antelopekeygen
