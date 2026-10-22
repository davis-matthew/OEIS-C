/*
 * A360482  E.g.f. satisfies A(x) = x * exp(x + 3 * A(x)).
 * https://oeis.org/A360482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} 3^(k-1) · k^(n-1) · binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t total = 0;
    seq_t pow3   = 1;                 /* 3^(k-1) */
    seq_t exp     = (seq_t)(n - 1);   /* exponent for k^(n-1) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t powk = big_pow((seq_t)k, exp);    /* k^(n-1)   */
        seq_t term = binomial(n, k);
        term = term * powk * pow3;             /* full term */
        total += term;                         /* accumulate */
        pow3 *= 3;                              /* next power of 3 */
    }
    return total;
}
