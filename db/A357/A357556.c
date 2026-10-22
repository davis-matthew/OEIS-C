/*
 * A357556  a(n) is the denominator of Sum_{d|n} (-1)^(d+1) / d^2.
 * https://oeis.org/A357556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^2 / gcd(n^2, | Σ_{d|n} (-1)^{d+1} (n/d)^2 | )
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator of Σ_{d|n} (-1)^{d+1}/d^2 for n ≥ 1. */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* defensive; sequence not defined for n=0 */

    /* sums of terms with odd (positive) and even (negative) divisors */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* iterate over all divisors of n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor i */
            idx_t d = i;
            seq_t t = ((seq_t)(n / d)) * ((seq_t)(n / d));
            if (d & 1)
                sum_pos += t;
            else
                sum_neg += t;

            /* complementary divisor n/i (avoid duplication when i==n/i) */
            idx_t d2 = n / i;
            if (d2 != d) {
                seq_t t2 = ((seq_t)(n / d2)) * ((seq_t)(n / d2));
                if (d2 & 1)
                    sum_pos += t2;
                else
                    sum_neg += t2;
            }
        }
    }

    /* absolute difference of the two sums */
    seq_t diff = (sum_pos >= sum_neg) ? (sum_pos - sum_neg) : (sum_neg - sum_pos);

    /* n^2 */
    seq_t n_sq = ((seq_t)n) * ((seq_t)n);

    /* reduce: a(n) = n^2 / gcd(n^2, diff) */
    seq_t g = gcd(n_sq, diff);
    return n_sq / g;
}
