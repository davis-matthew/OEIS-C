/*
 * A346939  a(n) = Sum_{d|n} mu(n/d) * binomial(8*d,d) / (7*d+1).
 * https://oeis.org/A346939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * C(8*d,d) / (7*d+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
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
    if (n == 0) return 0;
    __int128 sum_plus  = 0;
    __int128 sum_minus = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            int mu1 = mu_of(n / d1);
            if (mu1 != 0) {
                __int128 a_val = (__int128)8 * d1;
                __int128 bin   = 1;
                for (idx_t k = 1; k <= d1; ++k) {
                    bin = bin * (a_val - (__int128)d1 + (__int128)k) / (__int128)k;
                }
                __int128 denom = (__int128)(7 * d1 + 1);
                bin /= denom;               /* f(d1) */
                if (mu1 == 1) sum_plus += bin; else sum_minus += bin;
            }

            /* divisor d2 = n / i, if distinct from d1 */
            idx_t d2 = n / i;
            if (d2 != d1) {
                int mu2 = mu_of(n / d2);   /* n / d2 == i */
                if (mu2 != 0) {
                    __int128 a_val = (__int128)8 * d2;
                    __int128 bin   = 1;
                    for (idx_t k = 1; k <= d2; ++k) {
                        bin = bin * (a_val - (__int128)d2 + (__int128)k) / (__int128)k;
                    }
                    __int128 denom = (__int128)(7 * d2 + 1);
                    bin /= denom;               /* f(d2) */
                    if (mu2 == 1) sum_plus += bin; else sum_minus += bin;
                }
            }
        }
    }

    __int128 result = sum_plus - sum_minus;
    return (seq_t)result;
}
