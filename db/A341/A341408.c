/*
 * A341408  Number of partitions of n into 3 nonprime parts.
 * https://oeis.org/A341408
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341408: number of partitions of n into 3 non‑prime parts
   (unordered triples a ≤ b ≤ c with a + b + c = n and
   each part not prime).  Computed by a simple O(n²) search. */

#include <stdbool.h>
#include "../utils/int_sqrt.h"

static inline bool is_prime_local(idx_t x) {
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    idx_t r = int_sqrt(x);
    for (idx_t d = 5; d <= r; d += 6) {
        if (x % d == 0 || x % (d + 2) == 0) return false;
    }
    return true;
}

static inline bool is_nonprime(idx_t x) {
    return !is_prime_local(x);
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t cnt = 0;
    for (idx_t a = 1; a <= n / 3; ++a) {
        if (is_nonprime(a) == false) continue;
        idx_t max_b = (n - a) / 2;
        for (idx_t b = a; b <= max_b; ++b) {
            if (is_nonprime(b) == false) continue;
            idx_t c = n - a - b;
            if (c < b) break;
            if (is_nonprime(c) == false) continue;
            ++cnt;
        }
    }
    return cnt;
}
