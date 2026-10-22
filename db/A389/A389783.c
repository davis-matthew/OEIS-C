/*
 * A389783  Expansion of e.g.f. exp( -LambertW(-x*(1+x)^2) ) / (1+x).
 * https://oeis.org/A389783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(2*k-1, n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum   = 0;
    seq_t ratio = 1;               /* ratio = n! / k! for the current k */

    /* Compute terms for k = n .. 1 */
    for (idx_t k = n; k >= 1; k--) {
        seq_t pow_val = pow_uint((uint64_t)(k + 1), k - 1);      /* (k+1)^(k-1) */
        seq_t binom   = binomial(2 * k - 1, n - k);              /* C(2k-1, n-k) */

        seq_t term = ratio;
        term *= pow_val;
        term *= binom;

        sum += term;

        if (k > 1)
            ratio *= k;                             /* update to n!/(k-1)! */
    }

    /* After the loop, ratio equals n! */
    seq_t n_fact = ratio;

    /* Add the k = 0 term: n! * (-1)^n */
    if ((n & 1ULL) == 0)
        sum += n_fact;      /* even n  -> + n! */
    else
        sum -= n_fact;      /* odd n   -> - n! */

    return sum;
}
