/*
 * A396916  E.g.f. A(x) satisfies A(x) = 1 + x * exp(2*x) * A(x * exp(x)).
 * https://oeis.org/A396916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0}^{n‑1} (k+2)^(n-k-1) * binomial(n-1,k) * a(k)
 *            * Initial values: a(0) = 1, a(1) = 1, a(2) = 6, ... (see OEIS
 *            A396916)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */
#include <stddef.h>   /* size_t */

/* Memoised storage for the sequence and helper tables. */
static seq_t *A = NULL;        /* term array */
static seq_t *powk = NULL;     /* (k+2)^(exp) values for current n */
static seq_t *binom = NULL;    /* binomial(n-1,k) values for current n */
static size_t cap = 0;         /* allocated capacity */

/* Ensure that terms up to n are computed. */
static void ensure_A(idx_t n)
{
    if ((size_t)n < cap) return;        /* already available */

    /* Grow capacity to at least n+1, doubling each step. */
    size_t new_cap = cap ? cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    /* Reallocate the three arrays. */
    void *tmp;

    tmp = realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    A = (seq_t *)tmp;

    tmp = realloc(powk, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    powk = (seq_t *)tmp;

    tmp = realloc(binom, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    binom = (seq_t *)tmp;

    /* Compute new terms in order. */
    for (idx_t i = (idx_t)cap; i <= n; ++i)
    {
        if (i == 0) {
            A[0] = 1;
            continue;
        }

        /* Update powk: existing exponents increase by one. */
        if (i == 1) {
            powk[0] = 1;               /* (0+2)^0 = 1 */
        } else {
            for (idx_t k = 0; k < i - 1; ++k) {
                powk[k] *= (seq_t)(k + 2);
            }
            powk[i - 1] = 1;           /* new k has exponent 0 */
        }

        /* Compute binom(k) = binomial(i-1, k) for k=0..i-1. */
        if (i == 1) {
            binom[0] = 1;               /* binomial(0,0) */
        } else {
            idx_t new_n = i - 1;        /* n for current binomial */
            binom[0] = 1;
            for (idx_t k = 1; k < new_n; ++k) {
                /* binom(new_n, k) = binom(new_n, k-1) * (new_n-k+1)/k */
                binom[k] = binom[k - 1] * (seq_t)(new_n - k + 1) / (seq_t)k;
            }
            binom[new_n] = 1;          /* binomial(new_n, new_n) = 1 */
        }

        /* Sum over k=0 .. i-1 */
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += binom[k] * powk[k] * A[k];
        }

        A[i] = (seq_t)i * sum;
    }

    cap = (size_t)(n + 1);
}

/* Public function as required. */
seq_t a(idx_t n)
{
    ensure_A(n);
    return A[(size_t)n];
}
