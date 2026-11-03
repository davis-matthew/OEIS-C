/*
 * A291197  Second differences of frac(n/log(n)) rounded, n >= 2.
 * https://oeis.org/A291197
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(-(n+2)/log(n+2)) - 2*floor(-(n+1)/log(n+1)) +
 *            floor(-n/log(n))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9916 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* using natural logarithm, defined for n >= 2 */

#include <math.h>

static inline seq_t floor_neg(idx_t k)
{
    /* k >= 2 */
    long double ln = logl((long double)k);
    long double val = -(long double)k / ln;
    long long iv = (long long)floor(val);
    return (seq_t)iv;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n < 2, but return 0 to be safe */

    seq_t f_n   = floor_neg(n);
    seq_t f_np1 = floor_neg(n + 1);
    seq_t f_np2 = floor_neg(n + 2);

    return f_np2 - 2 * f_np1 + f_n;
}
