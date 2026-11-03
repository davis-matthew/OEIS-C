/*
 * A327278  a(n) = Sum_{d|n, d odd} d * mu(d) * mu(n/d).
 * https://oeis.org/A327278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy, changed
 * Formula:   a(n) = Sum_{d|n, d odd} d * mu(d) * mu(n/d)
 * Range:     i8 n <= 92, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8290 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i != 0)
            continue;

        /* divisor i */
        if ((i & 1) != 0)      /* i is odd */
        {
            int mu_i  = mu_of(i);
            int mu_j  = mu_of(n / i);
            sum += (seq_t)i * (seq_t)mu_i * (seq_t)mu_j;
        }

        /* complementary divisor j = n / i */
        idx_t j = n / i;
        if (j != i && (j & 1) != 0)   /* j is odd and distinct from i */
        {
            int mu_j  = mu_of(j);
            int mu_i  = mu_of(i);
            sum += (seq_t)j * (seq_t)mu_j * (seq_t)mu_i;
        }
    }
    return sum;
}
