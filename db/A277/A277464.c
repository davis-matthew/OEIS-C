/*
 * A277464  Expansion of e.g.f. cosh(x)/(1 + LambertW(-x)).
 * https://oeis.org/A277464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277464
   Formula: a(n) = Σ_{k=0..⌊n/2⌋} (n-2k)^(n-2k) * binomial(n,2k)   (OEIS)
   (The power 0^0 is defined as 1.) */

#include <stdbool.h>
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp)      */
#include "../utils/binomial.h"    /* binomial(idx_t n, idx_t k)          */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;     /* exponent of the term   */
        seq_t pow_val;

        /* 0^0 is treated as 1 for the sequence definition */
        if (m == 0) {
            pow_val = (seq_t)1;
        } else {
            pow_val = big_pow((seq_t)m, (seq_t)m);
        }

        seq_t bin_val = binomial(n, 2 * k);
        sum += pow_val * bin_val;
    }
    return sum;
}
