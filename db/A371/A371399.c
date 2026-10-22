/*
 * A371399  a(n) = 2^n * Sum_{k=0..n} binomial(k + n, k) * binomial(2*n - k,
 *          n) * (-1/2)^k.
 * https://oeis.org/A371399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * Σ_{k=0..n} binomial(k + n, k) * binomial(2*n - k,
 *            n) * (-1/2)^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Positive and negative partial sums to avoid unsigned underflow. */
    seq_t pos = 0;
    seq_t neg = 0;

    /* Current values of the binomial factors.  B = binomial(n + k, k),
       C = binomial(2*n - k, n).  The initial values correspond to k = 0. */
    seq_t B = 1;                                  /* binomial(n,0)   */
    seq_t C = binomial(2 * n, n);                /* binomial(2n,n)  */

    /* 2^n as a seq_t.  It will be halved after each iteration. */
    seq_t pow = (seq_t)1 << n;                      /* 1 << n = 2^n   */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = B * C * pow   (always positive) */
        seq_t term = B * C;
        term *= pow;

        if ((k & 1) == 0)
            pos += term;   /* even k => positive contribution   */
        else
            neg += term;   /* odd  k => negative contribution   */

        if (k == n)
            break;

        /* Update B to binomial(n + k + 1, k + 1) */
        seq_t tmpB = B * (seq_t)(n + k + 1);
        B = tmpB / (seq_t)(k + 1);

        /* Update C to binomial(2*n - (k+1), n) */
        seq_t tmpC = C * (seq_t)(n - k);
        C = tmpC / (seq_t)(2 * n - k);

        /* Halve 2^(n-k) to get 2^(n-(k+1)) */
        pow >>= 1;
    }

    /* Final result: pos - neg  (pos is guaranteed to be >= neg) */
    return pos - neg;
}
