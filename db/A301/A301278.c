/*
 * A301278  Numerator of variance of n-th row of Pascal's triangle.
 * https://oeis.org/A301278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   big_pow, binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = numerator of binomial(2n,n)/n - 4^n/(n*(n+1))
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t B   = binom((idx_t)(2 * n), (idx_t)n);   /* binomial(2n,n) */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);       /* 4^n          */
    seq_t t    = (seq_t)n + 1;                       /* n + 1        */

    seq_t num = B * t - pow4;                      /* B*(n+1) - 4^n */
    seq_t den = (seq_t)n * t;                      /* n*(n+1)       */

    seq_t g = gcd(num, den);                       /* reduce fraction */
    return num / g;                                /* numerator only */
}
