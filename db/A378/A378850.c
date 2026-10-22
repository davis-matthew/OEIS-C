/*
 * A378850  a(n) is the total number of paths starting at (0, 0), ending at
 *          (n, 0), consisting of steps (1, 1), (1, 0), (1, -2), and staying
 *          on or above y = -2.
 * https://oeis.org/A378850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1.
   For n > 0, a(n) counts walks from height 2 to height 2 in n steps,
   with step set {+1, 0, -2}, staying at heights >= 0.
   The values are computed incrementally using dynamic programming over the
   height variable.  The algorithm runs in O(n²) time but is fast enough
   for n up to ~10 000.  It stores intermediate results in static
   buffers so that subsequent calls (in any order) are answered in
   O(1).  All arithmetic is performed on the seq_t type supplied by the
   build system. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static buffers that persist between calls. */
    static seq_t *dp_cur   = NULL;   /* current step's height counts   */
    static seq_t *dp_next  = NULL;   /* next step's height counts    */
    static size_t  dp_cap   = 0;     /* capacity of dp arrays        */
    static seq_t *cache     = NULL;  /* cache of a values            */
    static idx_t   cache_cap= 0;     /* capacity of cache array      */
    static idx_t   cur_n    = 0;     /* highest n processed so far    */

    /* Ensure cache can hold values up to n */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap? cache_cap*2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) return 0;  /* out of memory */
        if (cache) {
            memcpy(new_cache, cache, sizeof(seq_t) * cache_cap);
            free(cache);
        }
        cache = new_cache;
        memset((void*)(cache + cache_cap), 0, sizeof(seq_t)*(new_cap - cache_cap));
        cache_cap = new_cap;

        /* initial value a(0) = 1 */
        cache[0] = (seq_t)1;
    }

    /* If we already know the answer, just return it */
    if (n <= cur_n) return cache[n];

    /* Ensure dp arrays are large enough for heights up to 2 + n */
    size_t needed_cap = 2 + (size_t)n + 3;   /* +3 to allow h+1 and h-2 indices */
    if (dp_cap < needed_cap) {
        size_t new_cap = dp_cap? dp_cap*2 : needed_cap;
        if (new_cap < needed_cap) new_cap = needed_cap;
        seq_t *new_dp_cur  = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        seq_t *new_dp_next = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        if (!new_dp_cur || !new_dp_next) return 0; /* out of memory */
        memset((void*)new_dp_cur,  0, sizeof(seq_t) * new_cap);
        memset((void*)new_dp_next, 0, sizeof(seq_t) * new_cap);
        if (dp_cur) {
            memcpy(new_dp_cur,  dp_cur,  sizeof(seq_t) * dp_cap);
            free(dp_cur);
        }
        if (dp_next) {
            memcpy(new_dp_next, dp_next, sizeof(seq_t) * dp_cap);
            free(dp_next);
        }
        dp_cur  = new_dp_cur;
        dp_next = new_dp_next;
        dp_cap  = new_cap;
    }

    /* If this is the very first call, initialise the dp array. */
    if (cur_n == 0) {
        memset((void*)dp_cur, 0, sizeof(seq_t)*dp_cap);
        dp_cur[2] = (seq_t)1;   /* start at height 2 with probability 1 */
    }

    /* Incrementally extend the computation to the requested n */
    for (idx_t step = cur_n + 1; step <= n; ++step) {
        /* zero dp_next up to the maximum possible height for this step */
        size_t max_h = 2 + (size_t)(step - 1);   /* highest height reachable so far */
        memset((void*)dp_next, 0, sizeof(seq_t) * (max_h + 3));

        /* propagate from current heights to next heights */
        for (size_t h = 0; h <= max_h; ++h) {
            seq_t val = dp_cur[h];
            if (!val) continue;
            /* step U : height +1 */
            dp_next[h + 1] += val;
            /* step H : height unchanged */
            dp_next[h]     += val;
            /* step D : height -2, only allowed if h >= 2 */
            if (h >= 2) dp_next[h - 2] += val;
        }

        /* swap arrays for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;

        /* store the result for this step */
        cache[step] = dp_cur[2];   /* a(step) is count at height 2 */
    }

    cur_n = n;
    return cache[n];
}
