/*
 * A295609  a(n) = least prime number p such that p AND n = n (where AND
 *          denotes the binary AND operator).
 * https://oeis.org/A295609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least prime p such that (p & n) == n
 * Range:     u8 n <= 243, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Special case for n = 0 – the smallest prime is 2, and 2 & 0 = 0. */
    if (n == 0) return (seq_t)2;

    /* Search for the smallest prime p >= n satisfying (p & n) == n. */
    for (uint64_t p = (uint64_t)n; ; ++p) {
        if ( ((seq_t)p & (seq_t)n) == (seq_t)n ) {
            if (is_prime((idx_t)p)) {
                return (seq_t)p;
            }
        }
    }
}
