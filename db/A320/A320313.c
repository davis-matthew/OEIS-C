/*
 * A320313  Number of ways to write n as the sum of 4 positive integers a, b,
 *          c, d such that d < b and a/b - c/d = (a - c)/(b - d).
 * https://oeis.org/A320313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (a,b,c,d) of positive integers with
 *            d < b, a + b + c + d = n, and a/b - c/d = (a - c)/(b - d)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Euclidean GCD for idx_t (unsigned 64‑bit). */
static idx_t my_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 4 no positive quadruple exists. */
    if (n < 4)
        return (seq_t)0;

    seq_t count = 0;

    /* d must satisfy 2d + 3 <= n  (minimal sum with b = d + 1,  a = c = 1). */
    idx_t d_max = (n >= 3) ? (n - 3) / 2 : 0;

    for (idx_t d = 1; d <= d_max; ++d)
    {
        /* b ranges from d+1 up to min(2d-1, n - d - 2).
           The extra bound ensures at least a,c >= 1. */
        idx_t b_min = d + 1;
        idx_t b_max = (2 * d) - 1;
        if (b_max > n - d - 2)
            b_max = n - d - 2;
        if (b_max < b_min)
            continue;

        idx_t k = d * d;   /* d^2 */

        for (idx_t b = b_min; b <= b_max; ++b)
        {
            /* X = b * (2d - b)  > 0 because d < b < 2d. */
            idx_t X = b * ((idx_t)2 * d - b);
            if (X == 0)          /* should not happen, but be safe */
                continue;

            idx_t g   = my_gcd(k, X);
            idx_t S   = X + k;           /* denominator factor */
            idx_t num = n - b - d;        /* n - (b + d) */

            if (num == 0)              /* would give t = 0 */
                continue;

            idx_t num_mul_g = num * g;
            if (num_mul_g % S != 0)
                continue;

            idx_t t = num_mul_g / S;
            if (t == 0)
                continue;                /* no positive solution */

            /* With these t, c and a are positive and integer by construction. */
            ++count;
        }
    }

    return (seq_t)count;
}
