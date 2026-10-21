/*
 * A242280  a(n) = Sum_{k=0..n} (k! * Stirling2(n,k))^3.
 * https://oeis.org/A242280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} (k! * Stirling2(n,k))^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/*  a(n)  */
seq_t a(idx_t n)
{
    /* static state: cache of already computed a(i) and two rows of
       T(n,k) = k! · Stirling2(n,k)  (k ≥ 0)                       */
    static seq_t *cache      = NULL;   /* cache[i] = a(i)          */
    static size_t  cache_len = 0;       /* number of cached entries  */

    static seq_t *row_prev   = NULL;   /* previous row T(n-1,·)    */
    static seq_t *row_cur    = NULL;   /* current row T(n,·)       */
    static size_t  prev_len  = 0;       /* length of row_prev        */
    static size_t  cur_len   = 0;       /* length of row_cur         */
    static idx_t   max_n_cached = 0;    /* largest n that has been computed */

    /* ---------- lazy initialization (for n = 0) ---------- */
    if (cache_len == 0) {
        cache = seq_alloc(1);           /* cache[0] = a(0) */
        cache[0] = (seq_t)1;
        cache_len = 1;

        row_prev = seq_alloc(1);        /* row_prev[0] = T(0,0)=1 */
        row_prev[0] = (seq_t)1;
        prev_len = 1;

        row_cur = seq_alloc(1);         /* buffer for the next row */
        cur_len = 1;

        max_n_cached = 0;
    }

    /* ---------- return cached value if possible ---------- */
    if (n < max_n_cached) {
        return cache[(size_t)n];
    }

    /* ---------- compute rows up to the requested n ---------- */
    for (idx_t i = max_n_cached + 1; i <= n; ++i) {
        size_t needed_len = (size_t)i + 1;   /* length of row i */

        /* enlarge row_cur if necessary */
        if (cur_len < needed_len) {
            row_cur = seq_realloc(row_cur, cur_len, needed_len);
            cur_len = needed_len;
        }

        /* Build the current row from the previous row                       */
        row_cur[0] = (seq_t)0;                       /* T(i,0) = 0 for i>0 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t left  = (k < (idx_t)prev_len) ? row_prev[(size_t)k]   : (seq_t)0;
            seq_t right = row_prev[(size_t)k - 1];
            row_cur[(size_t)k] = (seq_t)k * (left + right);
        }

        /* Compute a(i) = Σ_{k=1..i} T(i,k)^3  (k=0 term is zero) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t t = row_cur[(size_t)k];
            sum += t * t * t;
        }

        /* enlarge cache if needed */
        if (cache_len < (size_t)i + 1) {
            cache = seq_realloc(cache, cache_len, (size_t)i + 1);
            cache_len = (size_t)i + 1;
        }
        cache[(size_t)i] = sum;

        /* ---------- swap rows for the next iteration ---------- */
        seq_t *tmp = row_prev;
        row_prev  = row_cur;
        row_cur   = tmp;

        /* update lengths after the swap                          */
        size_t tmp_len = prev_len;
        prev_len       = cur_len;
        cur_len        = tmp_len;

        max_n_cached = i;
    }

    return cache[(size_t)n];
}
