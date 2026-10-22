/*
 * A308546  Number of double-closed subsets of {1..n}.
 * https://oeis.org/A308546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k < n/2} (2 + ⌊log₂(n/(2k+1))⌋)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 86, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, not used but harmless */

seq_t a(idx_t n)
{
    /* a(0) = 1 is covered by the loop below when n == 0,
       because the condition denom > n is true for k = 0. */
    seq_t prod = (seq_t)1;

    for (idx_t k = 0; ; ++k)
    {
        /* 2k + 1 is always odd and fits in idx_t for the given constraints */
        idx_t denom = (idx_t)(2 * k + 1);
        if (denom > n)
            break;

        idx_t q = n / denom;          /* q >= 1 because denom <= n             */
        unsigned l = 0;                /* floor(log2(q))                         */
        idx_t temp = q;
        while (temp > 1)
        {
            temp >>= 1;
            ++l;
        }

        seq_t mult = (seq_t)(2 + l);
        prod *= mult;
    }

    return prod;
}
