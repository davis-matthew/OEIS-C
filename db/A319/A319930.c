/*
 * A319930  a(n) = (1/24)*n*(n - 1)*(n - 3)*(n - 14).
 * https://oeis.org/A319930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/24)*n*(n - 1)*(n - 3)*(n - 14)
 * Range:     i8 n <= 15, i16 n <= 34, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t denom = (seq_t)24;
    seq_t res   = (seq_t)1;

    seq_t f0 = (seq_t)n;
    seq_t f1 = f0 - (seq_t)1;
    seq_t f2 = f0 - (seq_t)3;
    seq_t f3 = f0 - (seq_t)14;
    seq_t factors[4] = {f0, f1, f2, f3};

    for (int i = 0; i < 4; ++i) {
        seq_t sf = factors[i];
        seq_t av = sf < (seq_t)0 ? -sf : sf;
        seq_t g  = gcd(av, denom);
        sf      /= g;
        denom   /= g;
        res     *= sf;
    }

    /* After cancelling with 24, denom should be 1.  If not, divide once
       to handle any rounding that might occur in non-standard types. */
    if (denom != (seq_t)1)
        res /= denom;

    return res;
}
