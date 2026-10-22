/*
 * A390269  E.g.f. A(x) satisfies A(x) = exp( x / (1+x^2) * A(x)^2 ).
 * https://oeis.org/A390269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A390269):
   a(n) = n! * Σ_{k=0..⌊n/2⌋} (-1)^k * (2*(n‑2k)+1)^(n‑2k‑1) *
          binomial(n‑k‑1, k) / (n‑2k)!                                  */

#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Normalised parameters */
    idx_t maxk = n >> 1;          /* floor(n/2)                */
    seq_t fact_n      = factorial(n);   /* n!                         */
    seq_t sum_pos = 0, sum_neg = 0;     /* separate even / odd terms  */

    idx_t  s   = n;                 /* current s = n–2k            */
    seq_t  ratio = 1;               /* n! / s!  (starts with s=n) */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Base and exponent for the power term */
        seq_t base = (seq_t)(2 * s + 1);
        seq_t pow_term;
        if (s == 0)                               /* (2*0+1)^(-1) = 1   */
            pow_term = (seq_t)1;
        else
            pow_term = big_pow(base, (seq_t)(s - 1));

        /* Term = (n! / s!) * (2*s+1)^(s-1) * binomial(n-k-1, k)  */
        seq_t term = ratio;
        term *= pow_term;
        term *= binomial(n - k - 1, k);

        /* Accumulate with sign */
        if ((k & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;

        /* Prepare for next k (unless this was the last iteration) */
        if (k == maxk) break;

        /* ratio_new = ratio * s * (s-1)
         * (since r = (s+1)...n, r' = (s-1) s (s+1)...n          */
        ratio *= (seq_t)s * (seq_t)(s - 1);
        s -= 2;
    }

    /* Final value: sum_pos – sum_neg   (always non‑negative)   */
    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
