/*
 * A386685  Classification of the interval [4*n+1, 4*n+2, 4*n+3] according to
 *          positions of primes.
 * https://oeis.org/A386685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*A010051(4*n+1) + 2*A010051(4*n+2) + A010051(4*n+3)
 * Range:     u8 n <= 14641, u16 n <= 14641, u32 n <= 14641, u64 n <= 14641,
 *            u128 n <= 14641, big any n
 * Validated: standard, pass, 103 terms through n = 9130 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A010051(k) = 1 if k is prime, 0 otherwise. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Compute the three candidates in the interval [4*n+1, 4*n+2, 4*n+3] */
    idx_t v1 = (n << 2) + 1;   /* 4*n + 1 */
    idx_t v2 = (n << 2) + 2;   /* 4*n + 2 */
    idx_t v3 = (n << 2) + 3;   /* 4*n + 3 */

    seq_t result = 0;

    if (is_prime(v1)) result += (seq_t)4;
    if (is_prime(v2)) result += (seq_t)2;
    if (is_prime(v3)) result += (seq_t)1;

    return result;
}
