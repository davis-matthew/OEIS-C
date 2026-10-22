/*
 * A372316  Expansion of e.g.f. exp( x - LambertW(-3*x)/3 ).
 * https://oeis.org/A372316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (3*k+1)^(k-1) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
/* The arbitrary‑precision utility needed for the exponentiation */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t binom = 1;   /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term;
        if (k == 0)
            term = 1;                                 /* (3*0+1)^(-1) is treated as 1 */
        else
        {
            seq_t base = 3 * (seq_t)k + 1;
            term       = big_pow(base, (seq_t)(k - 1));
        }
        sum += term * binom;

        if (k == n)
            break;

        /* Update binom to C(n,k+1) from C(n,k) */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return sum;
}
