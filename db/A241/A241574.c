/*
 * A241574  Number of ternary words of length n avoiding the pattern 11-11.
 * https://oeis.org/A241574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-7)*(16 + 36*n - n^2 + n^3) for n>3 Base values:
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=27, a(4)=78, a(5)=222, a(6)=618,
 *            a(7)=1686
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 52, u128 n <= 112,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base values, sufficient for all n <= 7 */
    static const seq_t base[8] = {1, 3, 9, 27, 78, 222, 618, 1686};

    if (n < 8)
        return base[n];

    /* n >= 8 : use the closed‑form formula */
    seq_t m   = (seq_t)n;
    seq_t poly = 16 + (36 * m) - (m * m) + (m * m * m); /* 16 + 36n – n^2 + n^3 */

    seq_t factor = pow2(n - 7);                     /* 2^(n-7) */
    seq_t prod   = 3 * factor;                      /* 3 * 2^(n-7) */

    return prod * poly;
}
