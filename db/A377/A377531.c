/*
 * A377531  Expansion of e.g.f. 1/(1 - x^2 * exp(x))^2.
 * https://oeis.org/A377531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+1) * k^(n-2*k)/(n-2*k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;          /* accumulated result */
    seq_t coeff = 1;          /* n! / (n-2k)! for current k */

    idx_t max_k = n / 2;      /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        if (k > 0) {
            /* Update coeff multiply by the two new factors:
               [n-2k+1] and [n-2k+2] to extend the product range.   */
            seq_t lower1 = (seq_t)(n - 2 * k + 1);
            seq_t lower2 = (seq_t)(n - 2 * k + 2);
            coeff = coeff * lower1 * lower2;
        }

        idx_t exp   = n - 2 * k;                        /* n-2k */
        seq_t power = big_pow((seq_t)k, (seq_t)exp);   /* k^(n-2k)   */
        seq_t term  = coeff * ((seq_t)(k + 1)) * power; /* (k+1) * k^(n-2k) * coeff */

        sum = sum + term;
    }

    return sum;
}
