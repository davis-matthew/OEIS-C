/*
 * A360746  a(n) is the maximum number of locations 1..n-1 which can be
 *          reached starting from a(n-1), where jumps from location i to i +-
 *          a(i) are permitted (within 1..n-1); a(1)=1. See example.
 * https://oeis.org/A360746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 469, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition of A360746:
   a(1) = 1.
   For n >= 2, let S be the set of indices {1, …, n-1}.
   Consider the directed graph on S whose arcs are
   i → i ± a(i) (only arcs that stay inside S are kept).
   The value a(n) is the number of vertices reachable from
   the vertex n−1 in this graph.  The sequence values a(i)
   for i < n are already known when computing a(n).  This
   definition matches the first terms:
   1,1,2,3,4,4,5,5,5,7,8,8,8,9,9,12,10,10,12,10,12,13,13,13 …  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility: To reset visited flags without clearing whole array. */
static idx_t *visit_time = NULL;
static idx_t visit_curtime = 1;

/* Cache of computed a(n).  a_cache[0] unused. */
static seq_t *a_cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_max = 0;

/* Queue for BFS.  Reallocated as needed. */
static idx_t *queue = NULL;
static idx_t queue_capacity = 0;

/* Ensure the cache and auxiliary arrays are large enough to hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Reallocate a_cache. */
    seq_t *new_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    a_cache = new_cache;
    cache_size = n;

    /* Reallocate visit_time. */
    idx_t *new_vis = (idx_t *)realloc(visit_time, (n + 1) * sizeof(idx_t));
    if (!new_vis) abort();
    visit_time = new_vis;

    /* Reallocate queue. */
    if ((idx_t)queue_capacity < n) {
        idx_t *new_q = (idx_t *)realloc(queue, n * sizeof(idx_t));
        if (!new_q) abort();
        queue = new_q;
        queue_capacity = n;
    }
}

/* Main routine: computes a(n) on demand, using memoization. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    ensure_capacity(n);

    /* Compute new terms up to n if not already done. */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        /* Start at vertex cur-1. */
        idx_t start = cur - 1;
        /* Initialize BFS. */
        idx_t qh = 0, qt = 0;
        seq_t count = 1;                       /* keeps start vertex */
        visit_time[start] = visit_curtime;     /* mark visited */
        queue[qt++] = start;

        while (qh < qt) {
            idx_t v = queue[qh++];
            /* Fetch a(v); cast to idx_t so we can use in arithmetic. */
            idx_t dv = (idx_t)a_cache[v];

            /* first neighbor: v - dv */
            if (dv <= v) {
                idx_t to = v - dv;
                if (to >= 1 && to <= cur - 1 && visit_time[to] != visit_curtime) {
                    visit_time[to] = visit_curtime;
                    ++count;
                    queue[qt++] = to;
                }
            }

            /* second neighbor: v + dv */
            idx_t to2 = v + dv;
            if (to2 <= cur - 1 && visit_time[to2] != visit_curtime) {
                visit_time[to2] = visit_curtime;
                ++count;
                queue[qt++] = to2;
            }
        }

        /* Store the result */
        a_cache[cur] = (seq_t)count;
        ++visit_curtime;                              /* next round of visits */

        /* For the very first run, set a_cache[1] */
        if (cur == 2) a_cache[1] = (seq_t)1;
    }

    computed_max = n;
    return a_cache[n];
}
