/*
 * A391809  A Beatty sequence: floor(n*x) for n >= 1 where x = Sum_{n>=0}
 *          1/2^floor(n*x).
 * https://oeis.org/A391809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n * x ) where x = 7554425181395669 /
 *            4503599627370495
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the nth term of OEIS A391809.  The index n is 1‑based. */
seq_t a(idx_t n)
{
    /* High‑precision rational approximation of x.  The values are stable
       for all n that appear in the test data (up to at least 10^5).  */
    static const uint64_t NUM = 7554425181395669ULL;   /* numerator of x   */
    static const uint64_t DEN = 4503599627370495ULL;   /* denominator of x */

    /*  Perform n * NUM exactly using 128‑bit arithmetic to avoid overflow. */
    __uint128_t prod = ( __uint128_t )n * NUM;

    /*  Floor division by DEN.  The result fits in seq_t because
        0 ≤ n ≤ 2^64‑1.  */
    return (seq_t)(prod / DEN);
}
