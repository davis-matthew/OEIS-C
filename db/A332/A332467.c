/*
 * A332467  a(n) = n! * Sum_{d|n} mu(d) / (d!)^(n/d).
 * https://oeis.org/A332467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} mu(d) / (d!)^(n/d)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 196 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

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
    if (n == 0) return 0;          /* undefined for n < 1 */

    seq_t fac_n = factorial(n);
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        int mu_d = mu_of(d);
        if (mu_d != 0)
        {
            seq_t term = fac_n / ipow(factorial(d), (seq_t)(n / d));
            if (mu_d > 0)
                sum += term;
            else
                sum -= term;
        }

        idx_t e = n / d;
        if (e == d) continue;     /* handle square‑root divisor once */

        int mu_e = mu_of(e);
        if (mu_e != 0)
        {
            seq_t term = fac_n / ipow(factorial(e), (seq_t)(n / e));
            if (mu_e > 0)
                sum += term;
            else
                sum -= term;
        }
    }

    return sum;
}
