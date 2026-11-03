/*
 * A329805  Expansion of Product_{i>=1, j>=1} (1 + x^(i*j) + x^(2*i*j)).
 * https://oeis.org/A329805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G(x) = ∏_{i≥1,j≥1} (1 + x^(i*j) + x^(2*i*j)) = ∏_{m≥1} (1 + x^m
 *            + x^(2m))^{d(m)}, where d(m) is the number of positive divisors
 *            of m
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* helper: compute binomial coefficients up to max_n */
static void build_binom(size_t max_n, seq_t ***binom_out)
{
#if defined(__cplusplus)
    seq_t **binom = new seq_t*[max_n + 1];
    for (size_t i = 0; i <= max_n; ++i)
        binom[i] = new seq_t[max_n + 1]();      /* value‑initialized to 0 */
#else
    seq_t **binom = (seq_t**)malloc((max_n + 1) * sizeof(seq_t*));
    for (size_t i = 0; i <= max_n; ++i)
    {
        binom[i] = (seq_t*)calloc(max_n + 1, sizeof(seq_t));
    }
#endif
    for (size_t i = 0; i <= max_n; ++i)
    {
        binom[i][0] = (seq_t)1;
        binom[i][i] = (seq_t)1;
        for (size_t j = 1; j < i; ++j)
            binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
    }
    *binom_out = binom;
}

/* helper: free binomial table */
static void free_binom(size_t max_n, seq_t **binom)
{
    for (size_t i = 0; i <= max_n; ++i)
    {
#if defined(__cplusplus)
        delete [] binom[i];
#else
        free(binom[i]);
#endif
    }
#if defined(__cplusplus)
    delete [] binom;
#else
    free(binom);
#endif
}

seq_t a(idx_t n)
{
    /* local type definitions */
    size_t N = (size_t)n;
    if (N == 0) return (seq_t)1;   /* a(0) = 1 */

    /* ---------- divisor counts d[1..N] ----------
       Simple sieve: d[1] = 1, for i=1..N:
       for multiples j of i, increment d[j] */
    size_t *divcnt = (size_t*)calloc(N + 1, sizeof(size_t));
    divcnt[1] = 1;
    for (size_t i = 1; i <= N; ++i)
    {
        for (size_t j = i; j <= N; j += i)
        {
            if (j == 1) continue;          /* already set */
            ++divcnt[j];
        }
    }

    /* determine maximum d(m) for binomial table */
    size_t max_d = 0;
    for (size_t i = 1; i <= N; ++i)
        if (divcnt[i] > max_d) max_d = divcnt[i];

    /* precompute binomial coefficients up to max_d */
    seq_t **binom;   /* binom[n][k] for n<=max_d, k<=n */
    build_binom(max_d, &binom);

    /* ---------- DP arrays a_cur, a_next ----------
       Two arrays of length N+1; values stored in seq_t.
       Use dynamic allocation compatible with C and C++. */
#if defined(__cplusplus)
    seq_t *a_cur = new seq_t[N + 1]();
    seq_t *a_next = new seq_t[N + 1]();
#else
    seq_t *a_cur = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *a_next = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif
    a_cur[0] = (seq_t)1;   /* a(0) = 1 */

    /* temporary buffer for coefficients of the current factor */
    for (size_t m = 1; m <= N; ++m)
    {
        size_t d_m = divcnt[m];
        size_t max_k = N / m;
        size_t Smax = (2 * d_m < max_k ? 2 * d_m : max_k);
        /* allocate coeff[0..Smax] */
#if defined(__cplusplus)
        seq_t *coeff = new seq_t[Smax + 1]();
#else
        seq_t *coeff = (seq_t*)calloc(Smax + 1, sizeof(seq_t));
#endif
        coeff[0] = (seq_t)1;   /* coefficient of x^(0*m) */

        /* compute coefficients for S = 1..Smax */
        for (size_t S = 1; S <= Smax; ++S)
        {
            seq_t sum = (seq_t)0;
            size_t max_c = S / 2;
            for (size_t c = 0; c <= max_c; ++c)
            {
                size_t rem = S - 2 * c;
                if (rem > d_m - c) continue;
                seq_t v1 = binom[d_m][c];
                seq_t v2 = binom[d_m - c][rem];
                seq_t prod = seq_mul(v1, v2);
                sum += prod;
            }
            coeff[S] = sum;
        }

        /* new[j] = a_cur[j] * coeff[0] = a_cur[j] */
        for (size_t j = 0; j <= N; ++j)
            a_next[j] = a_cur[j];

        /* apply remaining S contributions */
        for (size_t S = 1; S <= Smax; ++S)
        {
            seq_t coeffS = coeff[S];
            if (coeffS == (seq_t)0) continue;
            size_t step = S * m;
            for (size_t j = step; j <= N; ++j)
                a_next[j] += seq_mul(a_cur[j - step], coeffS);
        }

        /* swap arrays for next iteration */
        seq_t *tmp = a_cur;
        a_cur = a_next;
        a_next = tmp;

        /* free coefficient buffer */
#if defined(__cplusplus)
        delete [] coeff;
#else
        free(coeff);
#endif
    }

    /* result is a_cur[N] */
    seq_t result = a_cur[N];

    /* free resources */
#if defined(__cplusplus)
    delete [] a_cur;
    delete [] a_next;
#else
    free(a_cur);
    free(a_next);
#endif
    free_binom(max_d, binom);
    free(divcnt);

    return result;
}
