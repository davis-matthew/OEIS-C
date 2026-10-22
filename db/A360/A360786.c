/*
 * A360786  Number of ways to place two dimers on an n-cube.
 * https://oeis.org/A360786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)*n*(1 - 2*n + n*2^(n-1)) (n > 1, a(1) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    /* 2^(n-1) and 2^(n-2) */
    seq_t s2_n1 = pow2(n - 1);
    seq_t s2_n2 = pow2(n - 2);

    /* Compute B = 1 - 2*n + n*2^(n-1) in a safe unsigned manner */
    seq_t term_n_s2 = (seq_t)n * s2_n1;   /* n * 2^(n-1)   */
    seq_t B = (seq_t)1 + term_n_s2;        /* 1 + n*2^(n-1) */
    B -= (seq_t)2 * (seq_t)n;              /* subtract 2*n   */

    /* a = 2^(n-2) * n * B */
    seq_t tmp = (seq_t)n * B;
    return s2_n2 * tmp;
}
