/*
 * A377740  E.g.f. satisfies A(x) = exp(x) * (1 + x * A(x))^2.
 * https://oeis.org/A377740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377740
   Formula: a(n) = n! * Sum_{k=0..n} (k+1)^(n-k-1) * binom(2*k+2,k)/(n-k)!
   Offset: 0
   Note: The implementation uses an iterative recurrence for binomial
         coefficients to avoid factorial overflows and computes powers
         with fast exponentiation (big_pow).  All arithmetic is in seq_t. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Return the sequence value a(n) for all n >= 0. */
seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = 0;          /* running sum of terms */
    seq_t bin = (seq_t)1;   /* binom(2*k+2, k), initially for k = 0 */
    seq_t P   = (seq_t)1;   /* falling factorial n!/(n-k)! for current k, starts with k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        if (k < n) {
            /* (k+1)^(n-k-1) with exponent >= 0 */
            seq_t pow = big_pow((seq_t)(k + 1), (seq_t)(n - k - 1));
            sum += bin * pow * P;
        } else { /* k == n: exponent -1, handled by division */
            sum += bin * P / (seq_t)(k + 1);
        }

        if (k < n) {
            /* Prepare values for the next iteration (k -> k+1) */
            P *= (seq_t)(n - k);   /* P_{k+1} = P_k * (n - k) */

            /* Update binomial coefficient:
               bin_{k+1} = bin_k * (2k+4)(2k+3) / [(k+1)(k+3)]   */
            seq_t num = bin * (seq_t)(2 * k + 4) * (seq_t)(2 * k + 3);
            seq_t den = (seq_t)(k + 1) * (seq_t)(k + 3);
            bin = num / den;
        }
    }

    return sum;
}
