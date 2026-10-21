/*
 * A274502  a(n) = 90*binomial(n-1,7) + 9*binomial(n-1,6).
 * https://oeis.org/A274502
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 90*binomial(n-1,7) + 9*binomial(n-1,6)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;                /* below offset, value irrelevant */
    idx_t k = n - 1;                    /* use n-1 in binomial formula */
    seq_t term1 = binomial(k, 7);       /* C(k,7) */
    seq_t term2 = binomial(k, 6);       /* C(k,6) */
    return 90 * term1 + 9 * term2;      /* 90*C(k,7) + 9*C(k,6) */
}
