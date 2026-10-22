/*
 * A345259  Denominator of 4/(1 + 1^2/(3 + 2^2/(5 + 3^2/(7 + ... +
 *          (n-1)^2/(2*n-1) )))).
 * https://oeis.org/A345259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 4/(1 + 1^2/(3 + 2^2/(5 + 3^2/(7 + ... +
 *            (n-1)^2/(2*n-1) ))))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Reduce a fraction num/den to lowest terms.  Assumes den > 0. */
static inline void reduce_fractions(seq_t *num, seq_t *den)
{
    seq_t g = gcd(*num, *den);
    if (g > 1) {
        *num /= g;
        *den /= g;
    }
}

/* Compute the denominator a(n).  a(0)=a(1)=a(2)=1. */
seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;

    /* Start with the innermost fraction (n-1)^2/(2*n-1). */
    idx_t i = n - 1;
    seq_t cur_num = (seq_t)i * (seq_t)i;
    seq_t cur_den = (seq_t)(2 * i + 1);
    reduce_fractions(&cur_num, &cur_den);

    /* Iterate outward from i = n-2 down to 1. */
    for (idx_t k = n - 2; k > 0; --k) {
        seq_t k2 = (seq_t)k * (seq_t)k;              /* k^2 */

        /* sum = (2*k+1) + cur_num/cur_den */
        seq_t sum_num = (seq_t)(2 * k + 1) * cur_den + cur_num;
        seq_t sum_den = cur_den;

        /* new fraction = k^2 / sum = k^2 * sum_den / sum_num */
        seq_t new_num = k2 * sum_den;
        seq_t new_den = sum_num;
        reduce_fractions(&new_num, &new_den);

        cur_num = new_num;
        cur_den = new_den;
    }

    /* Now cur_num/cur_den is the inner continued fraction after 1 + ... */
    /* Sum = 1 + cur_num/cur_den = (cur_den + cur_num)/cur_den */
    seq_t sum_num = cur_den + cur_num;
    seq_t sum_den = cur_den;
    reduce_fractions(&sum_num, &sum_den);

    /* Result = 4 / sum = 4 * sum_den / sum_num */
    seq_t final_num = (seq_t)4 * sum_den;
    seq_t final_den = sum_num;
    reduce_fractions(&final_num, &final_den);

    return final_den;
}
