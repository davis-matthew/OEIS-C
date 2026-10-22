/*
 * A355132  E.g.f. A(x) satisfies A(x) = 1 + 2 * (1 - exp(-x)) * A(2 * (1 -
 *          exp(-x))).
 * https://oeis.org/A355132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355132
 * Recurrence:
 *   a(0) = 1
 *   a(n) = Σ_{k=1..n} (-1)^(n-k) · k · 2^k · S(n,k) · a(k-1)
 * where S(n,k) are Stirling numbers of the second kind.
 *
 * This implementation builds a(n) on demand, memoising all computed
 * values up to the largest requested n.  All intermediate values are
 * kept in unsigned integer type ``seq_t`` (uint8_t … unsigned __int128
 * or an arbitrary‑precision class).  No floating point or math library
 * is used.
 *
 * Complexity: O(n²) additions/multiplications, with n up to ≈10000
 * within the time limits.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *S_prev = NULL;   /* previous row of Stirling numbers */
    static seq_t *S_curr = NULL;   /* current row */
    static seq_t *pow2   = NULL;   /* powers of two */
    static seq_t *A      = NULL;   /* sequence values a(k) */
    static size_t cap = 0;         /* allocated capacity (in entries) */
    static idx_t computed_up_to = 0; /* highest n for which a(n) is known */

    /* Initial allocation for n = 0 */
    if (cap == 0) {
        cap = 1;
        S_prev = (seq_t *)malloc(cap * sizeof(seq_t));
        S_curr = (seq_t *)malloc(cap * sizeof(seq_t));
        pow2   = (seq_t *)malloc(cap * sizeof(seq_t));
        A      = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!S_prev || !S_curr || !pow2 || !A) return 0; /* allocation failed */

        /* base values */
        S_prev[0] = 1;            /* S(0,0) = 1 */
        pow2[0]   = 1;            /* 2^0 */
        A[0]      = 1;            /* a(0) */
    }

    /* Grow tables if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = (size_t)n + 1;
        S_prev = (seq_t *)realloc(S_prev, new_cap * sizeof(seq_t));
        S_curr = (seq_t *)realloc(S_curr, new_cap * sizeof(seq_t));
        pow2   = (seq_t *)realloc(pow2,   new_cap * sizeof(seq_t));
        A      = (seq_t *)realloc(A,      new_cap * sizeof(seq_t));
        if (!S_prev || !S_curr || !pow2 || !A) return 0; /* allocation failed */

        /* initialise new powers of two */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            if (i == 0) pow2[0] = 1;
            else        pow2[i] = pow2[i - 1] * (seq_t)2;
        }
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t m = computed_up_to + 1; m <= n; ++m) {
        /* Compute Stirling numbers for row m and a_m in one pass */
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        S_curr[0] = 0; /* S(m,0) = 0 for m>0 */

        for (idx_t k = 1; k <= m; ++k) {
            /* Stirling recurrence: S(m,k) = k*S(m-1,k) + S(m-1,k-1) */
            seq_t term = 0;
            if (k <= m - 1) term = S_prev[k] * (seq_t)k; /* k*S_prev[k] */
            term += S_prev[k - 1];                        /* + S_prev[k-1] */
            S_curr[k] = term;

            /* Contribution to a_m */
            seq_t contrib = term;
            contrib *= A[k - 1];          /* * a(k-1) */
            contrib *= pow2[k];           /* * 2^k */
            contrib *= (seq_t)k;          /* * k */

            bool sign_pos = (((m - k) & 1U) == 0U);  /* (-1)^(m-k) positive? */
            if (sign_pos) pos_sum += contrib;
            else          neg_sum += contrib;
        }

        /* final value: pos_sum - neg_sum (guaranteed non‑negative) */
        seq_t a_m = pos_sum >= neg_sum ? pos_sum - neg_sum : (seq_t)0;
        A[m] = a_m;

        /* prepare for next row: swap prev and curr */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }

    computed_up_to = n;
    return A[n];
}
