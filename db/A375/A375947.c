/*
 * A375947  Expansion of e.g.f. 1 / (1 + 4 * log(1 - x))^(3/2).
 * https://oeis.org/A375947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375947 : Expansion of e.g.f. 1 / (1 + 4 * log(1 - x))^(3/2)
 * Formula: a(n) = Σ_{k=0..n} A000407(k) * |s(n,k)|
 * where A000407(k) = ∏_{i=1..k} (4 i + 2)  (A000407(0)=1)
 * and s(n,k) are signed Stirling numbers of the first kind.
 * The implementation uses one‑dimensional in‑place DP for
 * Stirling(·,·) and memoises the sequence values up to the
 * largest requested index.  It works for any seq_t/idx_t
 * that support the arithmetic operators used below.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of already computed a(n) values. */
static seq_t *a_cache  = NULL;      /* a(n) values */
static seq_t *st      = NULL;      /* current row of unsigned Stirling numbers of the first kind */
static seq_t *A_arr   = NULL;      /* A000407(k) values */
static idx_t  current_n = 0;       /* highest n that has been computed */

/* Ensure the arrays are large enough and initialise the base case. */
static void ensure_capacity(idx_t target)
{
    /* First time: create the arrays for n = 0. */
    if (!a_cache) {
        a_cache = (seq_t *)calloc(1, sizeof(seq_t));
        st      = (seq_t *)calloc(1, sizeof(seq_t));
        A_arr   = (seq_t *)calloc(1, sizeof(seq_t));
        if (!a_cache || !st || !A_arr) exit(1);
        a_cache[0] = (seq_t)1;  /* a(0) = 1 */
        st[0]      = (seq_t)1;  /* Stirling(0,0) = 1 */
        A_arr[0]   = (seq_t)1;  /* A000407(0) = 1 */
        current_n = 0;
    }

    if (target <= current_n) return;

    /* Expand all three arrays to size target+1. */
    size_t new_cap = (size_t)target + 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    st      = (seq_t *)realloc(st,      new_cap * sizeof(seq_t));
    A_arr   = (seq_t *)realloc(A_arr,   new_cap * sizeof(seq_t));
    if (!a_cache || !st || !A_arr) exit(1);

    /* Zero‑initialize new entries. */
    for (idx_t i = current_n + 1; i <= target; ++i) {
        a_cache[i] = (seq_t)0;
        st[i]      = (seq_t)0;
        /* A_arr[i] will be set below. */
    }

    /* Compute A000407 values for new indices. */
    for (idx_t k = current_n + 1; k <= target; ++k) {
        A_arr[k] = A_arr[k-1] * ( (seq_t)(4ULL * k + 2ULL) );
    }

    /* Compute a(n) and update the Stirling row for each new n. */
    for (idx_t n = current_n + 1; n <= target; ++n) {
        seq_t sum = (seq_t)0;
        /* Update Stirling numbers from high to low to avoid overwriting needed values. */
        for (idx_t k = n; k > 0; --k) {
            seq_t old_val = st[k];
            seq_t new_val = st[k-1] + ( (seq_t)(n-1) ) * old_val;
            st[k] = new_val;
            sum += A_arr[k] * new_val;
        }
        st[0] = (seq_t)0;          /* c(n,0)=0 for n>0 */
        a_cache[n] = sum;
    }

    current_n = target;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_cache[n];
}
