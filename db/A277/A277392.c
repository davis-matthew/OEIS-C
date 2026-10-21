/*
 * A277392  a(n) = n!*LaguerreL(n, -3*n).
 * https://oeis.org/A277392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*LaguerreL(n, -3*n) = n!*∑_{k=0..n}
 *            binomial(n,k)*3^k*n^k/k! = ∑_{k=0..n} binomial(n,k)*(3*n)^k *
 *            (n! / k!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* assume seq_t, idx_t and helpers are provided by the build environment */
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* n!  (used as s0) */
    seq_t fact = factorial(n);

    seq_t sum   = 0;          /* running total of all terms */
    seq_t comb  = 1;          /* binomial(n, k)   ,  comb_0 = 1   */
    seq_t fac_k = fact;       /* n!/k!           ,  fac_0 = n!    */
    seq_t p_k   = 1;          /* (3n)^k           ,  p_0 = 1     */

    seq_t base = (seq_t)3 * (seq_t)n;   /* 3n, same for all k */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = comb * fac_k * p_k   (each multiplication is exact) */
        seq_t term = comb;
        term = term * fac_k;
        term = term * p_k;
        sum += term;

        /* update for the next k, stop after last term */
        if (k == n) break;

        /* comb_{k+1} = comb_k * (n-k)/(k+1)   (exact division) */
        comb = comb * (seq_t)(n - k) / (seq_t)(k + 1);

        /* fac_{k+1} = fac_k / (k+1)   (exact) */
        fac_k = fac_k / (seq_t)(k + 1);

        /* p_{k+1}   = p_k * (3n) */
        p_k = p_k * base;
    }
    return sum;
}
