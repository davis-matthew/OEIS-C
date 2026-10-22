/*
 * A366193  For n >= 0, a(n) is the least x >= 0 such that x^2 + (x + 2*n)^2 +
 *          1 is prime.
 * https://oeis.org/A366193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least x ≥ 0 such that * x² + (x + 2·n)² + 1 is prime.
 *            * If 4·n² + 1 is prime then a(n) = 0. * The sequence is defined
 *            for n ≥ 0
 * Range:     u8 n <= 5, u16 n <= 185, u32 n <= 185, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* compute 2*n as a seq_t */
    seq_t two_n = (seq_t)n * (seq_t)2;

    /* first test x = 0: value = 4*n² + 1 */
    seq_t val0 = two_n * two_n + (seq_t)1;
    if (is_prime_big(val0))
        return (seq_t)0;  /* a(n) = 0 */

    /* otherwise search for the smallest x > 0 with the property */
    for (idx_t x = 1; ; ++x) {
        seq_t sx = (seq_t)x;
        seq_t term1 = sx * sx;
        seq_t term2 = (sx + two_n) * (sx + two_n);
        seq_t v = term1 + term2 + (seq_t)1;
        if (is_prime_big(v))
            return sx;   /* least x found */
    }
}
