/*
 * A286069  Independence and clique covering number of the n X n antelope
 *          graph.
 * https://oeis.org/A286069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n >= 24 Initial terms:
 *            a(1)=1, a(2)=4, a(3)=9, a(4)=16, a(5)=21, a(6)=24, a(7)=25,
 *            a(8)=36, a(9)=48, a(10)=60, a(11)=72, a(12)=84, a(13)=92,
 *            a(14)=98, a(15)=113, a(16)=132, a(17)=153, a(18)=168, a(19)=185,
 *            a(20)=200, a(21)=221, a(22)=242, a(23)=265, a(24)=288
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not used but safe */

    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* highest index already stored */
    static size_t capacity = 0;    /* allocated size (>= max_cached+1) */

    /* required capacity: at least n+1, but also at least 25 to hold
       the first 24 terms for recurrence */
    size_t req_cap = (size_t) (n + 1);
    if (req_cap < 25) req_cap = 25;

    /* allocate or grow the cache if necessary */
    if (!cache || req_cap > capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, req_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache = tmp;
#ifdef __cplusplus
        /* construct newly allocated elements (C++ only) */
        for (idx_t i = capacity; i < (idx_t)req_cap; ++i)
            new (&cache[i]) seq_t();
#endif
        capacity = req_cap;
    }

    /* initialise first 24 terms on first use */
    if (max_cached == 0) {
        static const unsigned long long init_vals[24] =
            {1,4,9,16,21,24,25,36,48,60,72,84,92,98,113,132,153,168,185,200,221,242,265,288};
        for (idx_t i = 1; i <= 24; ++i) {
            cache[i] = (seq_t)init_vals[i-1];
        }
        max_cached = 24;
    }

    /* compute further terms if needed */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i >= 24) {
            cache[i] = (seq_t)2 * cache[i-1] - (seq_t)2 * cache[i-3] + cache[i-4];
        }
    }
    if (n > max_cached) max_cached = n;

    return cache[n];
}
