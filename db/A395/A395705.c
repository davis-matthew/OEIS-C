/*
 * A395705  Expansion of g^5*(3*g-2)/(4-3*g)^4, where g = 1+x*g^4 is the g.f.
 *          of A002293.
 * https://oeis.org/A395705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Σ_{k=0..n} 3^(n-k) * binomial(4*n+3, k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute the sum Σ_{k=0..n} 3^(n-k) * binomial(4*n+3, k)
       by iterating k from 0 to n, maintaining the binomial
       coefficient and power of 3 multiplicatively. */
    seq_t sum, powCurr, binom;
    idx_t k, N;

    N = n * 4 + 3;                     /* 4*n + 3 */
    powCurr = pow_seq((seq_t)3, (int)n); /* 3^n */
    binom = (seq_t)1;                   /* binomial coefficient for k=0 */
    sum = powCurr;                      /* term for k=0 */

    for (k = 1; k <= n; ++k)
    {
        /* Update binomial coefficient:
           C(4n+3, k) = C(4n+3, k-1) * (4n+3-(k-1)) / k */
        binom = binom * (seq_t)(N - (k - 1));
        binom = binom / k;

        /* Move to the next power of 3: 3^(n-k) */
        powCurr = powCurr / 3;

        sum += binom * powCurr;
    }

    return ((seq_t)(n + 1)) * sum;
}
