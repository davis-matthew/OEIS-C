/*
 * A362702  Expansion of e.g.f. 1/(1 + LambertW(-x^2 * exp(x))).
 * https://oeis.org/A362702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} k^(n‑k) / (k! * (n‑2k)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t kmax = n >> 1;          /* floor(n/2) */
    seq_t sum = 0;

    /* coefficient for k = 0 is always 1 */
    seq_t coeff = (seq_t)1;

    /* term for k = 0: 0^(n) is 1 when n==0, otherwise 0 */
    if (n == 0)
        sum = coeff;              /* 1 * 1 */
    /* else sum stays 0, since 0^positive = 0 */

    /* iterate k = 1 .. kmax */
    for (idx_t k = 1; k <= kmax; ++k) {
        /* update coefficient for current k using the recurrence:
           coeff_k = coeff_{k-1} * (n - 2(k-1)) * (n - 2(k-1) - 1) / k
        */
        seq_t num1 = (seq_t)(n - 2 * (k - 1));
        seq_t num2 = (seq_t)(n - 2 * (k - 1) - 1);
        coeff = coeff * num1 * num2 / (seq_t)k;

        /* compute k^(n-k) */
        seq_t pow_val = big_pow((seq_t)k, (seq_t)(n - k));

        /* add term to the sum */
        sum += coeff * pow_val;
    }

    return sum;
}
