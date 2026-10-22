/*
 * A375954  Expansion of e.g.f. 1 / (3 - 2 * exp(x))^(5/2).
 * https://oeis.org/A375954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * Sum_{k=0..n} (2k+3)!! * Stirling2(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A dynamic DP that builds the Stirling numbers row by row and uses */
/* the double‑factorial weights (2k+3)!!, which are obtained by the */
/* recurrence W[0]=3,   W[k+1] = (2k+5)*W[k].                         */
/* The implementation memoises all values up to the largest n     */
/* requested, so subsequent calls are O(1) apart from the         */
/* unavoidable linear cost when extending the table.               */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *A = NULL;         /* memoised sequence values */
static seq_t *S = NULL;         /* current Stirling row (size = max_n+1) */
static seq_t *W = NULL;         /* weights (2k+3)!! (size = max_n+1) */
static size_t A_cap = 0;        /* capacity of A */
static size_t S_cap = 0;        /* capacity of S */
static size_t W_cap = 0;        /* capacity of W */
static idx_t    max_n = 0;      /* highest n for which A[n] is computed */

/* helper: enlarge an array to new_cap, preserving old contents            */
static void enlarge(size_t *cap, seq_t **arr, size_t new_cap)
{
    if (*cap < new_cap) {
        seq_t *old = *arr;
        *arr = (seq_t *)realloc(old, new_cap * sizeof(seq_t));
        /* initialise new slots to zero */
        if (new_cap > *cap) {
            memset(*arr + *cap, 0, (new_cap - *cap) * sizeof(seq_t));
        }
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialise tables on first call */
    if (A == NULL) {
        enlarge(&A_cap, &A, 1);
        A[0] = 1;                            /* a(0) = 1 */
    }
    if (S == NULL) {
        enlarge(&S_cap, &S, 1);
        S[0] = 1;                             /* S(0,0) = 1 */
    }
    if (W == NULL) {
        enlarge(&W_cap, &W, 1);
        W[0] = (seq_t)3;                     /* (2*0+3)!! = 3 */
    }

    /* If we already have the value, return it */
    if (n <= max_n) {
        return A[n];
    }

    /* Extend tables up to n */
    size_t new_cap = (size_t)(n + 1);
    enlarge(&A_cap, &A, new_cap);
    enlarge(&S_cap, &S, new_cap);
    enlarge(&W_cap, &W, new_cap);

    /* Compute new weights W[k] for k = max_n+1 .. n */
    for (idx_t k = (idx_t)max_n + 1; k <= n; ++k) {
        /* W[k] = W[k-1] * (2*k + 3) */
        seq_t factor = (seq_t)(2 * k + 3);
        W[k] = W[k - 1] * factor;
    }

    /* Build Stirling rows and compute a values for each new n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* Update Stirling numbers in S array (in‑place from high to low) */
        for (idx_t k = m; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        S[0] = (seq_t)0;                      /* S(m,0) = 0 for m>0 */

        /* Compute weighted sum: sum_{k=0..m} W[k] * S[k] */
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k) {
            sum = sum + W[k] * S[k];
        }

        /* Divide by 3 (exact division) */
        A[m] = sum / (seq_t)3;
    }

    max_n = n;
    return A[n];
}
