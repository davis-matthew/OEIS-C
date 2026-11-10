/*
 * A276589  Transpose of A276588.
 * https://oeis.org/A276589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,c) = Sum_{k=0..c} binomial(c,k)*(1+r+k)!, read by
 *            antidiagonals as T(0,d), T(1,d-1), ..., T(d,0); equivalently row
 *            r is the r-th first-difference of A001339
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A276589_term(idx_t r, idx_t c)
{
    seq_t fac = 1;
    idx_t i;
    for (i = 2; i <= r + 1; i++) fac *= (seq_t)i;   /* (1+r)! */
    seq_t bin = 1;
    seq_t sum = 0;
    idx_t k;
    for (k = 0; k <= c; k++) {
        sum += bin * fac;
        /* advance to k+1 */
        bin = bin * (seq_t)(c - k) / (seq_t)(k + 1);
        fac *= (seq_t)(r + k + 2);
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    idx_t off = n - d * (d + 1) / 2;
    return A276589_term(off, d - off);
}
