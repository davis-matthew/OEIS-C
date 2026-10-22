/*
 * A336020  a(0) = a(1) = a(2) = 1; a(n) = Sum_{k=0..n-3} Stirling2(n-3,k) *
 *          a(k).
 * https://oeis.org/A336020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-3} Stirling2(n-3,k) * a(k), with a(0) = a(1)
 *            = a(2) = 1 (OEIS A336020)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 16, u64 n <= 24, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc */

/* The function must be defined with external linkage */
seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *a_cache = NULL;    /* array of a(0) .. a(max_cached)    */
    static size_t a_cap = 0;         /* allocated capacity for a_cache     */
    static idx_t  max_cached = 2;    /* highest n for which a_cache is set */

    static seq_t *s_row = NULL;     /* Stirling numbers for current m      */
    static size_t s_cap = 0;        /* allocated capacity for s_row        */
    static idx_t  s_m   = 0;        /* current m (s_row has size m+1)      */

    /* ---- lazy initialisation ---- */
    if (a_cache == NULL) {
        /* allocate and initialise a_cache[0..2] = 1 */
        a_cap = 8;                     /* tiny initial capacity */
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) { abort(); }
        a_cache[0] = a_cache[1] = a_cache[2] = (seq_t)1;

        /* initialise first Stirling row (m = 0) */
        s_cap = 8;
        s_row = (seq_t*)malloc(s_cap * sizeof(seq_t));
        if (!s_row) { abort(); }
        s_row[0] = (seq_t)1;  /* S(0,0) = 1 */
        s_m = 0;
    }

    /* ---- handle trivial n < 3 ---- */
    if (n <= max_cached) {
        return a_cache[n];
    }

    /* ---- ensure a_cache capacity for all values up to n ---- */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 8;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* ---- main loop: compute a(max_cached+1 .. n) ---- */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        idx_t m = i - 3;                      /* we need S(m, k) */

        /* ---- expand Stirling row to m if needed ---- */
        while (s_m < m) {
            /* ensure capacity for new element at index s_m+1 */
            if ((size_t)(s_m + 2) > s_cap) {
                size_t new_cap = s_cap ? s_cap * 2 : 8;
                while ((size_t)(s_m + 2) > new_cap) new_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(s_row, new_cap * sizeof(seq_t));
                if (!tmp) { abort(); }
                s_row = tmp;
                s_cap = new_cap;
            }

            /* compute next Stirling row: S(m+1, k) for k=m+1..1 */
            s_row[s_m + 1] = (seq_t)0;                 /* S(m+1, m+1) before update */
            for (idx_t k = s_m + 1; k >= 1; --k) {
                /* use old values because we traverse backwards */
                s_row[k] = (seq_t)k * s_row[k] + s_row[k - 1];
            }
            s_row[0] = (seq_t)0;                      /* S(m+1, 0) = 0 */
            ++s_m;                                     /* now s_m = m+1 */
        }

        /* ---- compute a[i] = Σ_{k=0..m} S(m,k) * a[k] ---- */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k) {
            sum += s_row[k] * a_cache[k];
        }
        a_cache[i] = sum;
    }

    max_cached = n;
    return a_cache[n];
}
