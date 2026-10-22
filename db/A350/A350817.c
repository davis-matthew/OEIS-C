/*
 * A350817  Number of minimum total dominating sets in the 2 X n king graph.
 * https://oeis.org/A350817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k) = 4^k, * a(4k+1) = 2*k*4^k for k>0, a(1)=1, * a(4k+2) =
 *            (k+1)*(41*k+48)*4^k/8, * a(4k+3) = (5*k+9)*4^k
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          // special case for a(1)

    idx_t k   = n / 4;                    // floor division
    seq_t p   = pow2((idx_t)(2 * k));     // p = 4^k

    switch (n % 4) {
    case 0:                         /* a(4k) */
        return p;

    case 1:                         /* a(4k+1), k>0 */
        return p * (seq_t)(2 * k);

    case 2:                         /* a(4k+2) */
    {
        seq_t factor1 = (seq_t)(k + 1);
        seq_t factor2 = (seq_t)(41 * k + 48);
        seq_t num = factor1;
        num *= factor2;
        num *= p;
        return num / (seq_t)8;      /* division is exact */
    }

    case 3:                         /* a(4k+3) */
        return p * (seq_t)(5 * k + 9);
    }

    /* unreachable */
    return (seq_t)0;
}
