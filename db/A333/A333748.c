/*
 * A333748  Number of nonprime divisors of n that are <= sqrt(n).
 * https://oeis.org/A333748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _floor_isqrt_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) = the number of non‑prime divisors of n that are <= floor(sqrt(n)). */
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime_small.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t limit = _floor_isqrt_u64(n);
    seq_t count = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* d is a divisor; count it if it is not prime */
            if (!is_prime_small((seq_t)d)) {
                ++count;
            }
        }
    }
    return count;
}
