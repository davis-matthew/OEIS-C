/*
 * A264872  Array read by antidiagonals: T(n,m) = 2^n*(1+2^n)^m; n,m >= 0.
 * https://oeis.org/A264872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where T(n,m)=2^n*(1+2^n)^m, read by antidiagonals
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that (d+1)(d+2)/2 > n ≥ d(d+1)/2. */
    idx_t d = 0;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Number of elements before diagonal d. */
    unsigned __int128 prev128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* Offset within diagonal d. */
    idx_t offset = n - prev;       /* 0 ≤ offset ≤ d */

    /* Coordinates (i,j) of the element on the antidiagonal. */
    idx_t i = offset;
    idx_t j = d - offset;

    /* Compute T(i,j) = 2^i * (1+2^i)^j. */
    seq_t pow2i  = seq_pow((seq_t)2, i);          /* 2^i */
    seq_t base   = pow2i + (seq_t)1;              /* 1 + 2^i */
    seq_t powexp = seq_pow(base, j);              /* (1+2^i)^j */

    return pow2i * powexp;
}
