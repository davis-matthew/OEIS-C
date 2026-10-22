/*
 * A397670  a(n) = binomial(4*n-5,n).
 * https://oeis.org/A397670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(4*n-5, n)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the binomial coefficient safely using gcd reduction
   and __int128 intermediate multiplication to avoid overflow. */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = binomial(-5,0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* k = 4*n - 5 may be negative for n = 0,1 */
    seq_t k = (seq_t)4 * (seq_t)n - (seq_t)5;
    seq_t res = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t num = k - (seq_t)(i - (idx_t)1); /* k - i + 1 */
        seq_t den = (seq_t)i;

        /* Reduce res with den */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* Reduce num with remaining den */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* Multiply res by num using 128‑bit intermediate to stay within range */
        res = (seq_t)((__int128)res * num) / den;
    }

    return res;
}
