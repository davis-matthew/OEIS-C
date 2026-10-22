/*
 * A313615  Coordination sequence Gal.6.323.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) with initial values a(0)=1,
 *            a(1)=5, a(2)=10, a(3)=14, a(4)=20, a(5)=24, a(6)=30, a(7)=34,
 *            a(8)=40, a(9)=44, a(10)=49, a(11)=54, a(12)=59
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: holds precomputed values indexed by n */
    static seq_t *cache = NULL;     /* array of seq_t */
    static size_t cap = 0;          /* allocated capacity */
    static idx_t computed_len = 0;  /* number of values computed (>=13 after init) */

    /* first call: build initial table */
    if (!cache) {
        /* allocate space for first 13 terms */
        cap = 13;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = 1;  cache[1] = 5;  cache[2] = 10;
        cache[3] = 14; cache[4] = 20; cache[5] = 24;
        cache[6] = 30; cache[7] = 34; cache[8] = 40;
        cache[9] = 44; cache[10] = 49; cache[11] = 54;
        cache[12] = 59;
        computed_len = 13;
    }

    /* enlarge allocated buffer if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 13;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    if ((size_t)n + 1 > computed_len) {
        for (size_t k = computed_len; k <= (size_t)n; ++k) {
            /* recurrence: a(k) = a(k-1) + a(k-11) - a(k-12)   (k >= 13) */
            cache[k] = cache[k-1] + cache[k-11] - cache[k-12];
        }
        computed_len = (size_t)n + 1;
    }

    return cache[n];
}
