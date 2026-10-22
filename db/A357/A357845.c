/*
 * A357845  Numerators of the partial alternating sums of the reciprocals of
 *          the sum of divisors function (A000203).
 * https://oeis.org/A357845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{k=1..n} (-1)^(k+1) / sigma(k) (sigma =
 *            sum of divisors)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but harmless */

    /* Allocate array for sigma values (1..n). 0=unused. */
    seq_t *sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!sigma) return 0;          /* allocation failure, return 0 as fallback */

    /* initialize sigma to 0 */
    for (idx_t i = 0; i <= n; ++i) sigma[i] = 0;

    /* Sieve to compute sigma(k) for all k <= n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += val;
    }

    seq_t num = 0;   /* numerator of the cumulative sum */
    seq_t den = 1;   /* denominator (always positive) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t s = sigma[k];

        /* Compute new numerator: add or subtract sdenominator */
        seq_t new_num;
        if (k & 1) {                     /* odd k: + term */
            new_num = num * s + den;
        } else {                         /* even k: - term */
            new_num = num * s - den;     /* assumes new_num stays non‑negative */
        }

        seq_t new_den = den * s;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    free(sigma);
    return num;                         /* return the numerator of the reduced sum */
}
