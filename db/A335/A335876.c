/*
 * A335876  a(1) = 2, and for n > 1, a(n) = n + (n/p), where p is largest
 *          prime dividing n, A006530(n).
 * https://oeis.org/A335876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (n / p), where p is the largest prime divisor of n
 *            and a(1) = 2
 * Range:     u8 n <= 191, u16 n <= 9656, u32 n <= 9656, u64 n <= 9656,
 *            u128 n <= 9656, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n) {
    if (n == 1) return 2;

    /* Largest prime divisor of n (n is uint64_t). */
    uint64_t p = largest_prime_divisor((uint64_t)n);

    /* Update result in seq_t.  The division is performed in uint64_t,
       then promoted to seq_t. */
    return (seq_t)n + (seq_t)(n / p);
}
