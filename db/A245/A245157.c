/*
 * A245157  G.f.: Sum_{n>=0} x^n/((1+x)^(2*n+1)*(1 - (2*n+1)*x)).
 * https://oeis.org/A245157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} Σ_{i=0..n-k} (-1)^i * * binom(2k+i,i) *
 *            (2k+1)^{\,n-k-i}
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: power of a small base (2k+1) to a non‑negative exponent t */
static seq_t pow_small(seq_t base, idx_t t)
{
    seq_t result = 1;
    seq_t cur = base;
    while (t > 0) {
        if (t & 1) result *= cur;
        cur *= cur;
        t >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t t    = n - k;                       /* remaining exponent for i+j */
        seq_t base = (seq_t)(2 * k + 1);           /* 2k+1  < 2^16 for n<=10000  */

        /* Compute base^t once (fast exponentiation) */
        seq_t pow_cur = pow_small(base, t);

        seq_t binom   = 1;                         /* binom(2k+0,0) = 1   */
        seq_t possum  = 0;
        seq_t negsum  = 0;

        for (idx_t i = 0; i <= t; ++i) {
            seq_t term = binom * pow_cur;
            if ((i & 1) == 0)   /* even i → positive term */
                possum += term;
            else
                negsum += term;

            /* prepare for next i (if any) */
            if (i < t) {
                binom = binom * (seq_t)(2 * k + i + 1); /* numerator of binom */
                binom = binom / (seq_t)(i + 1);         /* denominator   */
                pow_cur = pow_cur / base;                /* reduce power by factor base */
            }
        }

        seq_t kterm = possum - negsum;   /* possum >= negsum for every k   */
        result += kterm;
    }

    return result;
}
