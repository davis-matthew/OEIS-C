/*
 * A350725  a(n) = Sum_{k=0..n} k! * k^(n-k) * Stirling1(n,k).
 * https://oeis.org/A350725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} k! * k^(n-k) * Stirling1(n,k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic DP for the signed Stirling
   numbers of the first kind and incremental computation of the
   sequence, caching all results up to the maximum n requested.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* ---------- static cache state -------------------------------------- */
    static seq_t *a_cache = NULL;      /* a_cache[i] = a(i) */
    static size_t  a_cap   = 0;         /* allocated capacity of a_cache   */
    static idx_t   max_n   = 0;         /* highest n for which a_cache written */

    static seq_t *s_prev = NULL;       /* Stirling numbers for current row */
    static idx_t   current_row = 0;    /* row index of s_prev              */

    /* ---------- initialise for n==0 ------------------------------------- */
    if (max_n == 0 && a_cache == NULL) {
        a_cache = mem_new(seq_t, 1);
        a_cap   = 1;
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */

        s_prev = mem_new(seq_t, 1);
        current_row = 0;
        s_prev[0] = (seq_t)1;           /* s(0,0) = 1 */
    }

    /* ---------- if n already computed ----------------------------------- */
    if (n <= max_n) {
        return a_cache[n];
    }

    /* ---------- extend a_cache if necessary ---------------------------- */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap > 0 ? a_cap << 1 : 1;
        while (new_cap <= (size_t)(n + 1)) {
            new_cap <<= 1;
        }
        seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* ---------- compute rows from (max_n+1) up to n --------------------- */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* allocate array for Stirling numbers of row m */
        seq_t *s_cur = mem_new(seq_t, (size_t)(m + 1));
        if (m > 0) {
            s_cur[0] = (seq_t)0;          /* s(m,0) = 0 for m>0 */
            for (idx_t k = m; k >= 1; --k) {
                seq_t val_prev_prev = (k-1 <= current_row) ? s_prev[k-1] : (seq_t)0;
                seq_t val_prev      = (k   <= current_row) ? s_prev[k]   : (seq_t)0;
                s_cur[k] = val_prev_prev - (seq_t)(m - 1) * val_prev;
            }
        }

        /* compute a(m) */
        seq_t sum  = (seq_t)0;
        seq_t fact = (seq_t)1;            /* fact = k! , updates each loop */

        for (idx_t k = 1; k <= m; ++k) {
            fact = fact * (seq_t)k;                        /* k!  */
            seq_t powk = ipow_u64_idx((uint64_t)k, m - k);     /* k^(m-k) */
            sum = sum + fact * powk * s_cur[k];
        }

        a_cache[m] = sum;

        /* free previous Stirling row and set up for next iteration */
        mem_free(s_prev);
        s_prev = s_cur;
        current_row = m;
    }

    max_n = n;
    return a_cache[n];
}
