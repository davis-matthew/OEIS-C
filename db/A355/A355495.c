/*
 * A355495  Expansion of Sum_{k>=0} (k^2 * x/(1 - x))^k.
 * https://oeis.org/A355495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k^(2k) * binomial(n-1,k-1) for n>0 a(0) = 1
 *            (The binomial is computed incrementally to keep the complexity
 *            O(n).)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include "../utils/big_pow.h"   /* for big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum   = 0;     /* accumulator for the sum */
    seq_t binom = 1;     /* binomial coefficient C(n-1, k-1); starts with k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute k^(2k) */
        seq_t power = big_pow((seq_t)k, (seq_t)(2 * k));

        /* add the term to the sum */
        sum += power * binom;

        /* update binomial to C(n-1, k) for next iteration */
        if (k < n) {
            seq_t numer = (seq_t)(n - k);        /* (n-1)-(k-1) = n-k */
            binom = binom * numer / (seq_t)k;    /* exact division */
        }
    }

    return sum;
}
