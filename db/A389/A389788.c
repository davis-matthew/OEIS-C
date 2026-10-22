/*
 * A389788  E.g.f. A(x) satisfies A(x) = exp(x^3 * (1+x)^3 * A(x)).
 * https://oeis.org/A389788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (k+1)^(k-1) * binomial(3*k, n-3*k)
 *            / k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the closed‑form expression.  For each k we
   compute the factor n! / k! (an integer), multiply it by the power
   (k+1)^(k-1) and by binomial(3*k, n-3*k), and accumulate the results.
   The division is performed only when it is exact, so no intermediate
   fraction appears.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n! */
    seq_t fac_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fac_n = fac_n * (seq_t)i;

    idx_t maxk = n / 3;
    seq_t sum = (seq_t)0;
    seq_t fac_k = (seq_t)1;        /* k! for the current k, start with 0! = 1 */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* binomial(3*k, n-3*k) */
        idx_t r = n - 3 * k;
        seq_t binom = binomial((idx_t)(3 * k), r);

        /* (k+1)^(k-1) */
        seq_t pow_val;
        if (k == 0)               /* contributes nothing when n>0, but
                                     keep the correct value for n==0 */
            pow_val = (seq_t)1;   /* 1^(-1) is treated as 1 */
        else
            pow_val = big_pow((seq_t)(k + 1), (seq_t)(k - 1));

        /* n! / k! */
        seq_t fac_n_div_fk = fac_n / fac_k;

        /* term = (n! / k!) * (k+1)^(k-1) * binomial(...) */
        seq_t term = fac_n_div_fk * pow_val;
        term = term * binom;

        sum += term;

        /* update k! for next iteration */
        fac_k = fac_k * (seq_t)(k + 1);
    }

    return sum;
}
