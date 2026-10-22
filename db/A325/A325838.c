/*
 * A325838  a(n) is the product of divisors of the n-th triangular number.
 * https://oeis.org/A325838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of divisors of the n-th triangular number T_n =
 *            n*(n+1)/2, so a(n) = ∏_{d|T_n} d
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 26,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* we need the integer square root for seq_t */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* triangular number as seq_t */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* product of divisors */
    seq_t res = 1;

    /* maximum divisor to check */
    seq_t sqrtT = floor_sqrt(T);
    idx_t max_i = (idx_t)sqrtT;

    for (idx_t i = 1; i <= max_i; ++i)
    {
        seq_t d = i;      /* cast i to seq_t */
        if ((T % d) == 0)
        {
            seq_t other = T / d;
            res = res * d;
            if (other != d)
                res = res * other;
        }
    }

    return res;
}
