/*
 * A355784  a(n) is the number of distinct primes of the form k*(n-k)+n where
 *          1 <= k < n.
 * https://oeis.org/A355784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes of the form k*(n - k) + n for 1
 *            ≤ k < n (OEIS A355784)
 * Range:     u8 n <= 31, u16 n <= 1464, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_small.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    idx_t half = n / 2;            /* iterate k = 1 .. n/2 (unique values) */
    seq_t cnt = 0;
    for (idx_t k = 1; k <= half; ++k) {
        seq_t v = (seq_t)k * (seq_t)(n - k) + (seq_t)n;
        if (is_prime_small(v))
            ++cnt;
    }
    return cnt;
}
