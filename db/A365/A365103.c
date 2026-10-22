/*
 * A365103  Number of distinct quartic residues x^4 (mod 4^n), x=0..4^n-1.
 * https://oeis.org/A365103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(4^n/15) + (n mod 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* If seq_t is an unsigned integer type that can
       fit 64-bit (i.e. sizeof <= 8), use a 128-bit
       intermediate to avoid overflow of 4^n. */
    if (sizeof(seq_t) <= 8)
    {
        unsigned __int128 x = 1;
        for (idx_t i = 0; i < n; ++i)
            x *= 4U;                /* 4^n in 128 bits */

        seq_t ceil_div = (seq_t)((x + 14U) / 15U);
        seq_t res = ceil_div + (seq_t)(n & 1U);
        return res;
    }
    else
    {
        /* For larger types (including arbitrary‑precision),
           compute 4^n directly using the provided pow4. */
        seq_t pow4n = pow4(n);                 /* 4^n   */
        seq_t ceil_div = (pow4n + (seq_t)14) / (seq_t)15;
        seq_t res = ceil_div + (seq_t)(n & 1);
        return res;
    }
}
