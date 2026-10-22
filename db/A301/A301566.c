/*
 * A301566  a(n) = Sum_{k=1..n-1} k*A088459(n, k).
 * https://oeis.org/A301566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{k=1}^{n-1} k * A088459(n,k)
 * where A088459(n,k) counts vertices at distance k from a fixed vertex
 * in the odd graph O_n.  Using the distance–intersection mapping:
 *
 *   let r = n-1
 *   if k is odd   :  t = (k-1)/2
 *   if k is even :  t = r - k/2
 *
 *   A088459(n,k) = C(r, t) * C(n, r-t)
 *
 * Here C(a,b) is the binomial coefficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    idx_t r = n - 1;

    /* Allocate arrays for binomial coefficients */
    seq_t *cR = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *cN = (seq_t *)malloc((r + 1) * sizeof(seq_t));

    if (!cR || !cN) {
        free(cR);
        free(cN);
        return (seq_t)0;  /* fallback on allocation failure */
    }

    /* Compute C(r, t) for t = 0..r */
    cR[0] = (seq_t)1;
    for (idx_t t = 1; t <= r; ++t)
        cR[t] = cR[t - 1] * (seq_t)(r - t + 1) / (seq_t)t;

    /* Compute C(n, s) for s = 0..r */
    cN[0] = (seq_t)1;
    for (idx_t s = 1; s <= r; ++s)
        cN[s] = cN[s - 1] * (seq_t)(n - s + 1) / (seq_t)s;

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= r; ++k) {
        idx_t t;
        if (k & 1)            /* odd k */
            t = (k - 1) / 2;
        else                  /* even k */
            t = r - (k / 2);

        /* r - t is within 1..r */
        seq_t count = cR[t] * cN[r - t];
        sum += count * (seq_t)k;
    }

    free(cR);
    free(cN);

    return sum;
}
