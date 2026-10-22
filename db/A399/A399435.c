/*
 * A399435  a(n) is the numerator of the probability that two heads will turn
 *          up in succession in tossing n times a coin.
 * https://oeis.org/A399435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(1 - F(n+2)/2^n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   fibonacci_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fibonacci_u64.h"
/* Computes the numerator of (2^n - F_{n+2}) / 2^n after reduction.  */

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1, return 0 for safety */

    /* Compute 2^n */
    seq_t pow2 = ((seq_t)1) << n;

    /* Compute F(n+2) */
    seq_t fib = fibonacci_u64(n + 2);

    /* Compute difference 2^n - F(n+2) */
    seq_t diff = pow2 - fib;
    if (diff == 0) return 0;          /* handles n = 1 where diff = 0 */

    /* Reduce by removing trailing zeros (divide by 2 until odd) */
    while ((diff & ((seq_t)1)) == 0) {
        diff >>= 1;
    }

    return diff;
}
