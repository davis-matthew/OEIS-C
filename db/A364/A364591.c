/*
 * A364591  G.f. satisfies A(x) = 1/(1-x) + x^4*A(x)^4.
 * https://oeis.org/A364591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n-k,3*k) * binomial(4*k,k)
 *            / (3*k+1)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;                     /* floor(n/4) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t nk     = n - k;               /* n - k   */
        idx_t threek = 3 * k;               /* 3*k     */

        seq_t t1 = binomial(nk, threek);    /* binom(n-k, 3k) */
        seq_t t2 = binomial(4 * k, k);      /* binom(4k, k)   */

        seq_t denom = (seq_t)(threek + 1);   /* 3k + 1 */

        t1 *= t2;                           /* (binom(n-k,3k) * binom(4k,k)) */
        t1 /= denom;                        /* divide by (3k+1) */

        sum += t1;                          /* accumulate */
    }
    return sum;
}
