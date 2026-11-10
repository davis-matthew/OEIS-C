/*
 * A383424  Indices k such that A003849(k) = 1 and A383422(k) = 1.
 * https://oeis.org/A383424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index k >= 0 (in increasing order) with A003849(k) =
 *            1 and A383422(k) = 1; computed by a direct search over k
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t A003849_fib[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155, 165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073, 4807526976, 7778742049, 12586269025, 20365011074, 32951280099, 53316291173, 86267571272, 139583862445, 225851433717, 365435296162, 591286729879, 956722026041, 1548008755920, 2504730781961, 4052739537881, 6557470319842, 10610209857723, 17167680177565, 27777890035288, 44945570212853, 72723460248141, 117669030460994, 190392490709135, 308061521170129, 498454011879264, 806515533049393};

static const int A003849_fib_count = (int)(sizeof(A003849_fib) / sizeof(A003849_fib[0]));

static int A003849_rec(int k, uint64_t pos) {
	if (k == 0) {
		return 0;
	} else if (k == 1) {
		return (pos == 0) ? 0 : 1;
	} else {
		uint64_t len_k_1 = A003849_fib[k];
		if (pos < len_k_1) {
			return A003849_rec(k - 1, pos);
		} else {
			return A003849_rec(k - 2, pos - len_k_1);
		}
	}
}

static int A003849_a(uint64_t n) {
	int k = 0;
	while (k + 1 < A003849_fib_count && A003849_fib[k + 1] <= n) {
		k++;
	}
	return A003849_rec(k, n);
}

static int A383422_a(uint64_t n) {
	uint64_t lengths[100];
	lengths[0] = 1;
	lengths[1] = 3;
	unsigned int i = 1;
	while (lengths[i] <= n && i < 92) {
		++i;
		lengths[i] = lengths[i - 1] + lengths[i - 2];
	}
	unsigned int k = i;
	uint64_t idx = n;
	while (k > 1) {
		if (idx < lengths[k - 1]) {
			--k;
		} else {
			idx -= lengths[k - 1];
			k -= 2;
		}
	}
	if (k == 0) {
		return 0;
	}
	return (idx == 0) ? 0 : 1;
}

seq_t a(idx_t n) {
	uint64_t count = 0;
	uint64_t k;
	for (k = 0;; k++) {
		if (A003849_a(k) == 1 && A383422_a(k) == 1) {
			count++;
			if (count == (uint64_t)n) {
				return (seq_t)k;
			}
		}
	}
}
