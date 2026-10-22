/*
 * A344726  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} (-1)^(j+1) * floor(n/j)^k.
 * https://oeis.org/A344726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) read by antidiagonals downwards, where T(n,k) =
 *            Sum_{j=1..n} (-1)^(j+1) * floor(n/j)^k
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   _diagonal_from_index, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include <stdint.h>
#include "../utils/_diagonal_from_index.h"

seq_t a(idx_t n)
{
    /* Map the linear index n to coordinates (off, d) of T(off, d-off+1). */
    idx_t idx = n;            /* copy for clarity */
    idx_t d   = _diagonal_from_index(idx);                    /* diagonal */
    idx_t prev = ((unsigned __int128)(d - 1) * d) / 2;       /* elements before this diag */
    idx_t off = idx - prev;                                 /* 1‑based offset in diag */
    idx_t nn = off;                       /* first argument of T */
    idx_t k = d - off + 1;                /* second argument of T */

    seq_t sum = 0;
    for (idx_t j = 1; j <= nn; ++j) {
        idx_t base = nn / j;                           /* floor(nn / j) */
        seq_t term = big_pow((seq_t)base, (seq_t)k);   /* base^k */
        if (j & 1)   /* odd j → plus */
            sum += term;
        else         /* even j → minus */
            sum -= term;          /* safe because intermediate sums never go negative */
    }
    return sum;
}
