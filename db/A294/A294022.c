/*
 * A294022  Sum of the differences of the larger and smaller parts in the
 *          partitions of n into two parts with the larger part prime.
 * https://oeis.org/A294022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n - 2i) * A010051(n - i) where
 *            A010051(m) = 1 if m is prime, else 0
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t sum = 0;
    idx_t start = (n + 1) / 2;          /* ceil(n/2) */
    for (idx_t p = start; p < n; ++p) { /* p = n - i */
        if (is_prime(p))
            sum += ((seq_t)2 * p - (seq_t)n);
    }
    return sum;
}
