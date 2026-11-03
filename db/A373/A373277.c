/*
 * A373277  Number of monic irreducible polynomials over GF(7) with fixed
 *          nonzero trace.
 * https://oeis.org/A373277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(7*n) * Σ_{d|n, gcd(d,7)=1} mu(d) * 7^(n/d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
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
    seq_t pos = 0;          /* Sum of positive contributions */
    seq_t neg = 0;          /* Sum of negative contributions */

    /* Enumerate divisors of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        /* Process d1 */
        if (d1 % 7 != 0) {
            int mu = mu_of(d1);
            if (mu != 0) {
                idx_t exp = n / d1 - 1;           /* exponent for 7^(n/d-1) */
                seq_t term = ipow(7, (seq_t)exp);
                if (mu > 0)
                    pos += term;
                else
                    neg += term;
            }
        }

        /* Process d2 if distinct from d1 */
        if (d2 != d1 && d2 % 7 != 0) {
            int mu = mu_of(d2);
            if (mu != 0) {
                idx_t exp = n / d2 - 1;
                seq_t term = ipow(7, (seq_t)exp);
                if (mu > 0)
                    pos += term;
                else
                    neg += term;
            }
        }
    }

    seq_t raw = pos - neg;   /* Expected to be positive */
    return raw / n;          /* Final division by n */
}
