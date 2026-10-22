/*
 * A338689  a(n) = Sum_{d|n} (-1)^(d-1) * (n/d)^n * binomial(d+n/d-2, d-1).
 * https://oeis.org/A338689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * (n/d)^n * binomial(d + n/d - 2,
 *            d-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n >= 1 per the OEIS offset */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* first divisor d1, j = n/d1 = d2 */
            {
                idx_t j = d2;
                seq_t term = big_pow((seq_t)j, (seq_t)n) *
                            binomial(d1 + j - 2, d1 - 1);
                if (d1 & 1)
                    sum_pos += term;
                else
                    sum_neg += term;
            }

            /* second divisor d2, only if distinct */
            if (d1 != d2) {
                idx_t d  = d2;
                idx_t j  = d1;
                seq_t term = big_pow((seq_t)j, (seq_t)n) *
                            binomial(d + j - 2, d - 1);
                if (d & 1)
                    sum_pos += term;
                else
                    sum_neg += term;
            }
        }
    }

    return sum_pos - sum_neg;
}
