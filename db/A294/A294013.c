/*
 * A294013  Sum of the differences of the larger and smaller parts in the
 *          partitions of 2n into two parts with the smaller part prime.
 * https://oeis.org/A294013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Sum_{p <= n} (n - p) where p ranges over the primes ≤
 *            n
 * Range:     u8 n <= 25, u16 n <= 568, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime(p))
            sum += (seq_t)(n - p);
    }
    return 2 * sum;
}
