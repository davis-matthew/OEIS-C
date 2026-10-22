/*
 * A334000  a(n) = (2*n+1)!! * Sum_{k=0..n} k/(2*k+1).
 * https://oeis.org/A334000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)!! * Sum_{k=0..n} k/(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* a(n) computes the nth term of OEIS A334000 for all n >= 0.
 * It uses only integer arithmetic, handling arbitrarily large
 * intermediate values with the supported seq_t type. */
seq_t a(idx_t n)
{
    /* Sum of k/(2*k+1) as a reduced fraction sum_num/sum_den */
    seq_t sum_num = 0;
    seq_t sum_den = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term_num = (seq_t)k;
        seq_t term_den = (seq_t)(2 * k + 1);

        /* Compute new numerator and denominator */
        seq_t new_num = (seq_t)((unsigned __int128)sum_num * term_den +
                                (unsigned __int128)term_num * sum_den);
        seq_t new_den = (seq_t)((unsigned __int128)sum_den * term_den);

        /* Reduce fraction */
        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* Compute (2*n+1)!! as a product of odd numbers */
    seq_t df = 1;
    for (idx_t k = 0; k <= n; ++k) {
        df = (seq_t)((unsigned __int128)df * (2 * k + 1));
    }

    /* Final value: a(n) = df * sum_num / sum_den */
    seq_t result = (seq_t)((unsigned __int128)df * sum_num / sum_den);
    return result;
}
