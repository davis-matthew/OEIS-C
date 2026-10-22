/*
 * A366300  Expansion of e.g.f. 1 / (-4 + Sum_{k=1..5} exp(-k*x)).
 * https://oeis.org/A366300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} (-1)^(k+1) * binomial(n,k) * (1 +
 *            2^k + 3^k + 4^k + 5^k) * a(n-k). (OEIS A366300)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* allocate arrays a[0..n] and S[0..n] (S[0] unused) */
    seq_t *a_arr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *S = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a_arr || !S)
    {
        /* memory allocation failed */
        if (a_arr) free(a_arr);
        if (S) free(S);
        return (seq_t)0;
    }

    a_arr[0] = (seq_t)1;

    /* Precompute S[k] = 1 + 2^k + 3^k + 4^k + 5^k for k=1..n */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = 0;
        for (int base = 1; base <= 5; ++base)
            sum += pow_uint((uint64_t)base, k);
        S[k] = sum;
    }

    /* Main DP recurrence */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t pos = 0, neg = 0;
        seq_t bin = 1; /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k)
        {
            /* Update binomial coefficient C(i,k) */
            bin = (bin * (seq_t)(i - k + 1)) / (seq_t)k;

            /* term = bin * S[k] * a[i-k] */
            seq_t term = bin * S[k];
            term = term * a_arr[i - k];

            if (k & 1)   /* k odd => positive sign */
                pos += term;
            else
                neg += term;
        }

        /* a_arr[i] = pos - neg  (pos >= neg for all i) */
        a_arr[i] = pos - neg;
    }

    seq_t value = a_arr[n];

    free(a_arr);
    free(S);
    return value;
}
