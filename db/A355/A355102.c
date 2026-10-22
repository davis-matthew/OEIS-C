/*
 * A355102  E.g.f. A(x) satisfies A(x) = 1 + 2 * x * A(1 - exp(-x)).
 * https://oeis.org/A355102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(n)=2*n*Σ_{k=0..n-1} (-1)^(n-k-1) * Stirling2(n-1,k) *
 *            a(k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoised arrays for a(n) and Stirling numbers of the second kind
   for the current row.  All allocations grow only as needed. */

static seq_t *a_cache = NULL;     /* a(0), a(1), ... */
static size_t a_cap   = 0;        /* allocated capacity of a_cache     */
static size_t a_len   = 0;        /* number of a-values actually stored */

static seq_t *S = NULL;          /* S(n-1, k) for the next a(n) to compute */
static size_t S_cap   = 0;        /* allocated capacity of S             */
static size_t S_len   = 0;        /* length of S (currently: n of a to be computed) */

/* Ensure the a_cache array can hold 'needed' elements */
static void ensure_a_capacity(size_t needed)
{
    if (a_cap < needed) {
        size_t new_cap = a_cap ? a_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

/* Ensure the S array can hold 'needed' elements */
static void ensure_S_capacity(size_t needed)
{
    if (S_cap < needed) {
        size_t new_cap = S_cap ? S_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        S = tmp;
        S_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Very first call: initialise the cache with a(0)=1 and S(0,0)=1 */
    if (a_len == 0) {
        ensure_a_capacity(1);
        a_cache[0] = (seq_t)1;
        a_len = 1;

        ensure_S_capacity(1);
        S[0] = (seq_t)1;
        S_len = 1;
    }

    /* If already computed, just return */
    if (n < a_len) return a_cache[n];

    /* Compute all a(k) up to index n, keeping S in sync */
    for (idx_t cur = a_len; cur <= n; ++cur) {
        /* S currently holds S(cur-1, k) for k=0..cur-1 */
        seq_t sum = 0;

        for (idx_t k = 0; k < cur; ++k) {
            seq_t term = a_cache[k] * S[k];
            /* Parity: (-1)^(cur-k-1) */
            if (((cur - k - 1u) & 1u) != 0u)
                term = -term;
            sum += term;
        }

        seq_t new_val = (seq_t)2 * (seq_t)cur * sum;
        ensure_a_capacity(cur + 1);
        a_cache[cur] = new_val;

        /* Update S to row cur (i.e., Stirling numbers S(cur, k)) */
        ensure_S_capacity(cur + 1);
        if (S_len <= cur) {          /* add the new high index */
            S[cur] = (seq_t)0;
            S_len = cur + 1;
        }
        for (idx_t j = cur; j >= 1; --j) {
            seq_t old_j    = S[j];   /* old S(cur-1, j) */
            seq_t old_jm1  = S[j - 1];/* old S(cur-1, j-1) */
            S[j] = (seq_t)j * old_j + old_jm1;
        }
        S[0] = (seq_t)0;             /* S(cur,0)=0 for cur>0 */
    }

    a_len = n + 1;   /* we have computed up to a(n) */
    return a_cache[n];
}
