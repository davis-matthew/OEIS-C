/*
 * A367078  E.g.f. satisfies A(x) = 1 + log(1 + x*A(x)^2).
 * https://oeis.org/A367078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*n)! * Sum_{k=0..n} Stirling1(n,k)/(2*n-k+1)!
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A367078. Supports 64‑bit and arbitrary‑precision seq_t. */

#include <stdlib.h>

/* Ensure *arr has capacity for at least needed elements. */
static void ensure_capacity_seq_t(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0 ? *cap * 2 : 16);
    if (new_cap < needed) new_cap = needed;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static memoization state. */
    static seq_t *a_cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;        /* highest n whose a(n) is cached */

    static seq_t *st_prev = NULL;      /* previous Stirling row */
    static seq_t *st_curr = NULL;      /* current Stirling row */
    static size_t cap_prev = 0, cap_curr = 0;

    /* Initialise cache for n = 0. */
    if (cache_cap == 0) {
        ensure_capacity_seq_t(&a_cache, &cache_cap, (size_t)1);
        a_cache[0] = 1;
        cache_max = 0;
    }

    /* Return previously computed value if available. */
    if (n <= cache_max) return a_cache[n];

    /* Initialise Stirling rows if needed. */
    if (st_prev == NULL) {
        ensure_capacity_seq_t(&st_prev, &cap_prev, (size_t)1);
        st_prev[0] = 1;                /* s(0,0) = 1 */
    }
    if (st_curr == NULL) {
        ensure_capacity_seq_t(&st_curr, &cap_curr, (size_t)1);
    }

    /* Compute a(k) for all k from cache_max+1 up to n. */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        ensure_capacity_seq_t(&st_curr, &cap_curr, (size_t)m + 1);

        /* Build Stirling row s(m,k). */
        if (m == 0) {
            st_curr[0] = 1;
        } else {
            st_curr[0] = 0;                  /* s(m,0) = 0 for m>0 */
            for (idx_t k = 1; k < m; ++k) {
                seq_t term1 = st_prev[k - 1];
                seq_t term2 = st_prev[k];
                st_curr[k] = term1 - (seq_t)(m - 1) * term2;
            }
            st_curr[m] = 1;                  /* s(m,m) = 1 */
        }

        /* Compute a(m) from the formula. */
        seq_t a_val;
        if (m == 0) {
            a_val = 1;
        } else {
            seq_t sum = 0;
            seq_t prod = 1;
            for (idx_t k = 1; k <= m; ++k) {
                sum += st_curr[k] * prod;
                prod *= (seq_t)(2 * m - k + 1);
            }
            a_val = sum;
        }

        /* Cache the result. */
        ensure_capacity_seq_t(&a_cache, &cache_cap, (size_t)m + 1);
        a_cache[m] = a_val;
        cache_max = m;

        /* Prepare for next iteration: st_prev <-- st_curr, st_curr <-- old st_prev. */
        seq_t *tmp = st_prev;
        st_prev = st_curr;
        st_curr = tmp;
        size_t tmp_cap = cap_prev;
        cap_prev = cap_curr;
        cap_curr = tmp_cap;
    }

    return a_cache[n];
}
