/*
 * A364984  E.g.f. satisfies A(x) = 1 + x*A(x)^3*exp(x*A(x)).
 * https://oeis.org/A364984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(n+2*k+1,k) /
 *            ((n+2*k+1)*(n-k)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Compute the OEIS sequence A364984. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;             /* a(0) = 1 */

    seq_t sum  = (seq_t)0;
    seq_t prod = (seq_t)1;                         /* prod = n!/(n-k)! for current k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update prod = n!/(n-k)!  */
        prod = prod * (seq_t)(n - k + 1);

        /* k^(n-k) */
        seq_t powk = big_pow((seq_t)k, (seq_t)(n - k));

        /* binomial coefficient (n + 2k + 1 choose k) */
        seq_t bin = binomial((idx_t)(n + 2 * k + 1), (idx_t)k);

        /* denominator (n + 2k + 1) */
        seq_t denom = (seq_t)(n + 2 * k + 1);

        /* term = (prod * powk * bin) / denom */
        seq_t numerator = prod * powk * bin;
        seq_t term = numerator / denom;

        sum += term;
    }

    return sum;   /* this equals a(n) */
}
