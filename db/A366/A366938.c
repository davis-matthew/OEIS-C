/*
 * A366938  a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+2,3) * floor(n/k).
 * https://oeis.org/A366938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+2,3) * floor(n/k)
 * Range:     i8 n <= 10, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial(k+2,3) = (k+2)(k+1)k/6 without intermediate overflow */
static inline seq_t binom_kplus2_choose3(idx_t k)
{
    seq_t a = (seq_t)k;
    seq_t b = a + 1;
    seq_t c = a + 2;

    /* divide by 2 */
    if ((a & 1) == 0) a >>= 1;
    else if ((b & 1) == 0) b >>= 1;
    else c >>= 1;

    /* divide by 3 */
    if (a % 3 == 0) a /= 3;
    else if (b % 3 == 0) b /= 3;
    else c /= 3;

    return a * b * c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety, offset starts at 1 */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;                /* floor(n/k) */
        seq_t bin = binom_kplus2_choose3(k);
        seq_t term = bin * (seq_t)t;
        if (k & 1)                     /* odd k: (+) */
            sum += term;
        else                           /* even k: (-) */
            sum -= term;
    }
    return sum;
}
