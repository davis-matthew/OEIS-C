/*
 * A391985  a(n) = Sum_{k=0..floor(n/4)} (k+1) * 2^k * 3^(n-3*k) *
 *          binomial(n-2*k,k) * binomial(n-3*k,k).
 * https://oeis.org/A391985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} (k+1) * 2^k * 3^(n-3k) * binomial(n-2k,k)
 *            * binomial(n-3k,k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* upper bound for k is floor(n/4) */
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        /* term = (k+1) * 2^k * 3^(n-3k) * binomial(n-2k,k) * binomial(n-3k,k) */
        seq_t term = (seq_t)(k + 1);           /* k+1 */
        term *= pow2(k);                      /* * 2^k */
        term *= pow3(n - 3 * k);              /* * 3^(n-3k) */
        term *= binomial(n - 2 * k, k);       /* * binomial(n-2k,k) */
        term *= binomial(n - 3 * k, k);       /* * binomial(n-3k,k) */
        sum += term;                          /* accumulate */
    }
    return sum;
}
