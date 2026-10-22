/*
 * A343279  a(n) = Stirling2(n, floor(n/2)).
 * https://oeis.org/A343279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stirling2(n, floor(n/2))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Stirling numbers of the second kind for all k up to n
   and stores a(n) in a cache. Works for any seq_t, including
   arbitrary‑precision types.                                               */

#include <stdlib.h>
#include <stddef.h>

/* static state for memoisation */
static seq_t *S = NULL;        /* current row of Stirling numbers */
static size_t capk = 0;        /* capacity of S */
static seq_t *a_vals = NULL;   /* cached a(n) values */
static size_t capn = 0;        /* capacity of a_vals */
static size_t max_n_computed = 0;  /* largest n that has been processed */

/* Ensure S has capacity at least 'new_cap' (in elements).  */
static void ensure_capk(size_t new_cap) {
    if (new_cap <= capk)
        return;
    size_t old_cap = capk;
    capk = new_cap;
    seq_t *tmp = (seq_t *)realloc(S, capk * sizeof(seq_t));
    if (!tmp)
        exit(1);
    S = tmp;
    /* zero new elements */
    for (size_t i = old_cap; i < capk; ++i)
        S[i] = 0;
}

/* Ensure a_vals has capacity at least 'new_cap' (in elements). */
static void ensure_capn(size_t new_cap) {
    if (new_cap <= capn)
        return;
    size_t old_cap = capn;
    capn = new_cap;
    seq_t *tmp = (seq_t *)realloc(a_vals, capn * sizeof(seq_t));
    if (!tmp)
        exit(1);
    a_vals = tmp;
    /* zero new elements */
    for (size_t i = old_cap; i < capn; ++i)
        a_vals[i] = 0;
}

seq_t a(idx_t n) {
    /* Treat idx_t as size_t for array indexing (idx_t is unsigned). */
    size_t target_n = (size_t)n;

    /* Initialise storage on first call. */
    if (max_n_computed == 0) {
        ensure_capk(1);          /* need S[0] */
        S[0] = 1;                /* S(0,0)=1 */
        ensure_capn(1);          /* need a_vals[0] */
        a_vals[0] = (seq_t)1;    /* a(0)=1 */
        max_n_computed = 0;
    }

    /* Resize value cache if necessary. */
    if (target_n + 1 > capn)
        ensure_capn(target_n + 1);

    /* Compute rows from max_n_computed+1 up to target_n. */
    for (size_t m = max_n_computed + 1; m <= target_n; ++m) {
        /* Ensure row array large enough. */
        ensure_capk(m + 1);               /* indices 0..m */

        /* Update Stirling numbers S(m,k) for all k=m..1 (descending). */
        for (size_t k = m; k > 0; --k) {
            S[k] = ((seq_t)k) * S[k] + S[k - 1];
        }
        S[0] = 0;                         /* S(m,0)=0 for m>0 */

        /* Store the required value a(m) = S(m, floor(m/2)). */
        size_t k_needed = m / 2;
        a_vals[m] = S[k_needed];
    }

    max_n_computed = target_n;
    return a_vals[target_n];
}
