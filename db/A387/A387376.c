/*
 * A387376  a(n) = 2 + 4^n + 2*n*(n - 1).
 * https://oeis.org/A387376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 4^n + 2*n*(n - 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1, behaviour for n<1 irrelevant */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n); /* compute 4^n exactly */
    seq_t nn  = (seq_t)n;
    seq_t res = pow4 + (seq_t)2
                + (seq_t)2 * nn * (nn - 1);
    return res;
}
