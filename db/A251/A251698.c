/*
 * A251698  a(n) = (6*n+1) * (7*n+1)^(n-2) * 8^n.
 * https://oeis.org/A251698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n+1) * (7*n+1)^(n-2) * 8^n (offset 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* compute (6*n+1) and (7*n+1) */
    seq_t factor = (seq_t)6 * n + 1;
    seq_t base   = (seq_t)7 * n + 1;

    /* (7*n+1)^(n-2) , n >= 2 so exponent is non‑negative */
    seq_t pow1 = big_pow(base, (seq_t)(n - 2));

    /* 8^n */
    seq_t pow2 = big_pow((seq_t)8, (seq_t)n);

    return factor * pow1 * pow2;
}
