/*
 * A267499  Number of fixed points of autobiographical numbers (A267491 ...
 *          A267498) in base n.
 * https://oeis.org/A267499
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 2^(n-4) + n*(n-1)/2 for n >= 5 initial values: a(2)=2,
 *            a(3)=7, a(4)=7
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    if (n == 2)
        return 2;
    if (n == 3)
        return 7;
    if (n == 4)
        return 7;

    /* compute 2^(n-4) by repeated doubling (works for any seq_t) */
    seq_t pow2 = 1;
    for (idx_t i = 5; i <= n; ++i)
        pow2 = pow2 * 2;

    /* compute n*(n-1)/2 */
    seq_t tri = (seq_t)n * ((seq_t)n - 1) / 2;

    return pow2 + tri;
}
