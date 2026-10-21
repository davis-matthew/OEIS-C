/*
 * A278396  Number of positive meanders (walks starting at the origin and
 *          ending at any altitude > 0 that never touch or go below the x-axis
 *          in between) with n steps from {-4,-3,-2,-1,1,2,3,4}.
 * https://oeis.org/A278396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   array_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positive meanders with step set {-4,-3,-2,-1,1,2,3,4} */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/array_malloc.h"

/* ---------------------------------------------------------------------- */
/* Helper to allocate arrays of seq_t in a way that works in both C and C++ */

static seq_t* array_realloc(seq_t* old, size_t oldcap, size_t newcap)
{
    seq_t* newp = array_malloc(newcap);
    if (!old) return newp;
    size_t mincap = oldcap < newcap ? oldcap : newcap;
    for (size_t i = 0; i < mincap; ++i) newp[i] = old[i];
#ifdef __cplusplus
    delete[] old;
#else
    free(old);
#endif
    return newp;
}

static void array_free(seq_t* p)
{
    if (!p) return;
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ---------------------------------------------------------------------- */
/* Memoised DP state                                                     */
/* ---------------------------------------------------------------------- */
static seq_t *cum = NULL;                 /* cumulative results a(0)..a(n)      */
static idx_t  cum_capacity = 0;           /* number of elements in cum         */
static seq_t *dp_cur = NULL;              /* distribution at current step      */
static seq_t *dp_next = NULL;             /* temporary buffer for next step    */
static size_t dp_capacity = 0;            /* number of elements in dp buffers  */
static idx_t  computed_n = 0;             /* largest n already computed        */

/* ---------------------------------------------------------------------- */
/* Main function                                                        */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Allocate/resize cumulative array                                  */
    /* ------------------------------------------------------------------ */
    if (cum == NULL) {
        cum_capacity = (size_t)n + 1;
        cum = array_malloc(cum_capacity);
        if (!cum) abort();
        cum[0] = (seq_t)1;          /* empty walk */
    } else if ((size_t)n + 1 > cum_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t* tmp = array_realloc(cum, cum_capacity, new_cap);
        if (!tmp) abort();
        cum = tmp;
        /* zero new entries */
        for (size_t i = cum_capacity; i < new_cap; ++i) cum[i] = (seq_t)0;
        cum_capacity = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* Allocate/resize DP buffers                                         */
    /* ------------------------------------------------------------------ */
    size_t dp_needed = 4ull * (size_t)n + 1;   /* maximum altitude + 1 */
    if (dp_cur == NULL) {
        dp_capacity = dp_needed;
        dp_cur  = array_malloc(dp_capacity);
        dp_next = array_malloc(dp_capacity);
        if (!dp_cur || !dp_next) abort();
        for (size_t i = 0; i < dp_capacity; ++i) {
            dp_cur[i]  = (seq_t)0;
            dp_next[i] = (seq_t)0;
        }
        dp_cur[0] = (seq_t)1;   /* start at altitude 0 */
    } else if (dp_capacity < dp_needed) {
        size_t old_cap = dp_capacity;
        dp_capacity = dp_needed;
        seq_t* new_cur  = array_realloc(dp_cur,  old_cap, dp_capacity);
        seq_t* new_next = array_realloc(dp_next, old_cap, dp_capacity);
        if (!new_cur || !new_next) abort();
        dp_cur  = new_cur;
        dp_next = new_next;
        /* new slots are already zeroed by array_malloc */
    }

    /* ------------------------------------------------------------------ */
    /* Fast return if already computed                                   */
    /* ------------------------------------------------------------------ */
    if (computed_n >= n) return cum[n];

    /* ------------------------------------------------------------------ */
    /* Extend computation from computed_n+1 up to n                       */
    /* ------------------------------------------------------------------ */
    for (idx_t step = computed_n + 1; step <= n; ++step) {
        size_t cur_max_h  = 4ull * (size_t)(step - 1);
        size_t next_max_h = 4ull * (size_t)step;

        /* zero dp_next for the range we will write into it */
        for (size_t h = 0; h <= next_max_h; ++h) dp_next[h] = (seq_t)0;

        /* transition from each existing altitude */
        for (size_t h = 0; h <= cur_max_h; ++h) {
            seq_t val = dp_cur[h];
            if (val == (seq_t)0) continue;

            /* positive steps +1 .. +4 (always allowed) */
            if (h + 1 <= next_max_h) dp_next[h + 1] += val;
            if (h + 2 <= next_max_h) dp_next[h + 2] += val;
            if (h + 3 <= next_max_h) dp_next[h + 3] += val;
            if (h + 4 <= next_max_h) dp_next[h + 4] += val;

            /* negative steps: result must stay > 0 */
            if (h > 1) dp_next[h - 1] += val;
            if (h > 2) dp_next[h - 2] += val;
            if (h > 3) dp_next[h - 3] += val;
            if (h > 4) dp_next[h - 4] += val;
        }

        /* sum all final altitudes > 0 (positive meanders) */
        seq_t sum = (seq_t)0;
        for (size_t h = 1; h <= next_max_h; ++h) sum += dp_next[h];
        cum[step] = sum;

        /* swap buffers for next iteration */
        seq_t* tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    computed_n = n;
    return cum[n];
}
