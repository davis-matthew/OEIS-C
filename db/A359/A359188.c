/*
 * A359188  a(n) = Sum_{d|n} mu(n/d) * d * (n/d)^(d-1), where mu() is the
 *          Moebius function (A008683).
 * https://oeis.org/A359188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * d * (n/d)^(d-1)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Offset starts at 1 */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d1;

            /* divisor d1 */
            int mu1 = mu_of(n / d1);   /* μ(n/d1) */
            if (mu1 != 0)
            {
                seq_t pow1 = ipow((seq_t)(n / d1), (seq_t)(d1 - 1));
                seq_t term1 = (seq_t)mu1 * (seq_t)d1 * pow1;
                sum += term1;
            }

            /* divisor d2 (distinct from d1) */
            if (d2 != d1)
            {
                int mu2 = mu_of(n / d2);   /* μ(n/d2) */
                if (mu2 != 0)
                {
                    seq_t pow2 = ipow((seq_t)(n / d2), (seq_t)(d2 - 1));
                    seq_t term2 = (seq_t)mu2 * (seq_t)d2 * pow2;
                    sum += term2;
                }
            }
        }
    }

    return sum;
}
