/*
 * A332712  a(n) = Sum_{d|n} mu(d/gcd(d, n/d)).
 * https://oeis.org/A332712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} mu(d/gcd(d, n/d))
 * Range:     u8 n <= 2092, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9722 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

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
    /* Sum over all divisors d of n */
    idx_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* First divisor d = i */
            idx_t d1 = i;
            seq_t g1 = gcd((seq_t)d1, (seq_t)(n / d1));
            idx_t q1 = (idx_t)((seq_t)d1 / g1);
            sum += (idx_t)mu_of(q1);

            /* Second divisor d = n / i, if distinct */
            idx_t d2 = n / i;
            if (d1 != d2) {
                seq_t g2 = gcd((seq_t)d2, (seq_t)(n / d2));
                idx_t q2 = (idx_t)((seq_t)d2 / g2);
                sum += (idx_t)mu_of(q2);
            }
        }
    }
    return (seq_t)sum;
}
