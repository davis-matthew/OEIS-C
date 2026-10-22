/*
 * A364072  Triangle read by rows: T(n, k) = Sum_{d=0..n-k} binomial(n,
 *          d)*StirlingS2(n-d, k)*63^(n-d-k), with 0 <= k <= n.
 * https://oeis.org/A364072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten(row n, column k of T(n,k)), where T(n,k) =
 *            Σ_{d=0}^{n-k} C(n,d)*S₂(n-d,k)*63^{n-d-k}
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 28, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   flat_index_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* basic includes */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/flat_index_row.h"

/* main sequence function */
seq_t a(idx_t n_index)
{
    /* 1. map flattened index to (row n, column k) */
    idx_t n = flat_index_row(n_index);
    idx_t pre = (idx_t)((__int128)n * (n + 1) / 2);
    idx_t k = n_index - pre;

    /* 2. trivial case */
    if (k == 0)
        return (seq_t)1;

    if (k > n)
        return (seq_t)0;     /* out‑of‑range, should not happen */

    /* 3. compute binomial C(n,k) iteratively to avoid large factorials */
    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        binom = (binom * (seq_t)(n - i + 1)) / (seq_t)i;
    }

    /* 4. allocate and initialise Stirling array for t = k */
    seq_t *S = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (!S)
        return (seq_t)0;     /* memory failure, unlikely */

    /* build Stirling row for n = k */
    /* start with n = 0 row */
    for (idx_t j = 0; j <= k; ++j)
        S[j] = 0;
    S[0] = 1;
    for (idx_t t = 1; t <= k; ++t) {
        for (idx_t j = t; j >= 1; --j)
            S[j] = S[j] * (seq_t)j + S[j - 1];
        S[0] = 0;
    }

    /* 5. sum the terms, updating binomial and Stirling as we move m = k .. n */
    seq_t sum = 0;
    seq_t pow63 = 1;
    idx_t m = k;
    while (1) {
        sum += binom * S[k] * pow63;    /* current term */
        if (m == n)
            break;                    /* finished */

        /* prepare for next step (m+1) */
        binom = (binom * (seq_t)(n - m)) / (seq_t)(m + 1);
        pow63 *= 63;

        /* update Stirling array: S[j] = j*S[j] + S[j-1] (for j = k..1) */
        for (idx_t j = k; j >= 1; --j)
            S[j] = S[j] * (seq_t)j + S[j - 1];

        ++m;
    }

    free(S);
    return sum;
}
