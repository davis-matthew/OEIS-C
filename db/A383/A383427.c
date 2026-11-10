/*
 * A383427  Indices k such that A003849(k) = A383422(k).
 * https://oeis.org/A383427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 (in increasing order) with A003849(k) =
 *            A383422(k); computed by scanning k upward, caching found indices
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint64_t A383427_fib[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155, 165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073ULL, 4807526976ULL, 7778742049ULL, 12586269025ULL, 20365011074ULL, 32951280099ULL, 53316291173ULL, 86267571272ULL, 139583862445ULL, 225851433717ULL, 365435296162ULL, 591286729879ULL, 956722026041ULL, 1548008755920ULL, 2504730781961ULL, 4052739537881ULL, 6557470319842ULL, 10610209857723ULL, 17167680177565ULL, 27777890035288ULL, 44945570212853ULL, 72723460248141ULL, 117669030460994ULL, 190392490709135ULL, 308061521170129ULL, 498454011879264ULL, 806515533049393ULL};

static const int A383427_fib_count = (int)(sizeof(A383427_fib) / sizeof(A383427_fib[0]));

static int A383427_fibword(int k, uint64_t pos) {
	while (k > 1) {
		uint64_t l = A383427_fib[k];
		if (pos < l) {
			k = k - 1;
		} else {
			pos -= l;
			k = k - 2;
		}
	}
	if (k == 0) return 0;
	return (pos == 0) ? 0 : 1;
}

static int A383427_fib_at(uint64_t n) {
	int k = 0;
	while (k + 1 < A383427_fib_count && A383427_fib[k + 1] <= n) k++;
	return A383427_fibword(k, n);
}

static int A383427_lucas_at(uint64_t n) {
	uint64_t len[100];
	unsigned i = 1;
	len[0] = 1;
	len[1] = 3;
	while (len[i] <= n) {
		++i;
		len[i] = len[i - 1] + len[i - 2];
	}
	unsigned k = i;
	uint64_t idx = n;
	while (k > 1) {
		if (idx < len[k - 1]) {
			--k;
		} else {
			idx -= len[k - 1];
			k -= 2;
		}
	}
	if (k == 0) return 0;
	return (idx == 0) ? 0 : 1;
}

static uint64_t *A383427_cache = 0;
static uint64_t A383427_count = 0;
static uint64_t A383427_cap = 0;
static uint64_t A383427_next = 0;

seq_t a(idx_t n) {
	uint64_t want = (uint64_t)n;
	while (A383427_count < want) {
		uint64_t k = A383427_next++;
		if (A383427_fib_at(k) == A383427_lucas_at(k)) {
			if (A383427_count == A383427_cap) {
				A383427_cap = A383427_cap ? A383427_cap * 2 : 1024;
				A383427_cache = (uint64_t *)realloc(A383427_cache, A383427_cap * sizeof(uint64_t));
			}
			A383427_cache[A383427_count++] = k;
		}
	}
	return (seq_t)A383427_cache[want - 1];
}
