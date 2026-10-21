/*
 * A268533  Pascal's difference pyramid read first by blocks and then by rows:
 *          T(n,k,m) = 1/(m!) * (d/dx)^m((1-x)^k*(1+x)^(n-k))|_{x=0}.
 * https://oeis.org/A268533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Pascal's difference pyramid read by blocks
    a(n) is the n‑th element obtained by iterating
    over blocks of size (b+1)^2 and taking T(b,k,m)
    with k = offset/(b+1) and m = offset%(b+1).  */

#include <stddef.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Find the block index b such that the cumulative
       number of elements up to block b-1 does not exceed n. */
    idx_t cum = 0;
    idx_t b   = 0;
    for (;;) {
        idx_t sz  = b + 1;            /* square‑root of block size */
        idx_t blk = sz * sz;          /* number of elements in block b */
        if (cum + blk > n) break;
        cum += blk;
        ++b;
    }

    idx_t sz     = b + 1;
    idx_t offset = n - cum;
    idx_t k      = offset / sz;        /* row (k) inside block */
    idx_t m      = offset % sz;        /* column (m) inside block */

    /* Compute T(b,k,m) = sum_{i=0}^{min(k,m)} (-1)^i
                           binom(k,i) * binom(b-k, m-i). */
    seq_t sum = 0;
    idx_t upper = k < m ? k : m;
    for (idx_t i = 0; i <= upper; ++i) {
        seq_t term = binom(k, i) * binom(b - k, m - i);
        if (i & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
