/*
 * A331769  El Gradechi's hybrid coefficients alpha^{4,6}_n.
 * https://oeis.org/A331769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * 240 * binomial(n+5,5) - 504 * binomial(n+3,3)
 * Range:     i8 none, i16 n <= 4, i32 n <= 60, i64 n <= 5401, i128 n <= 10000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"


/* Compute binomial(n+k, k) with small k using safe reduction. */
static seq_t binom_small(idx_t n, idx_t k)
{
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = (seq_t)(n + i);             /* current numerator term   */
        seq_t g   = gcd(num, (seq_t)i);          /* cancel factor i from num  */
        num /= g;
        seq_t denom = (seq_t)i / g;              /* remaining divisor part   */
        seq_t g2   = gcd(res, denom);           /* cancel with current res  */
        res /= g2;
        res *= num;
    }
    return res; /* equals C(n+k, k) */
}

seq_t a(idx_t n)
{
    seq_t t1 = binom_small(n, (idx_t)5); /* C(n+5,5) */
    seq_t t2 = binom_small(n, (idx_t)3); /* C(n+3,3) */

    seq_t part1 = (seq_t)240 * t1;
    seq_t part2 = (seq_t)504 * t2;

    seq_t sign = (n & (idx_t)1) ? (seq_t)-1 : (seq_t)1;

    return sign * part1 - part2;
}
