/*
 * A359884  Number of 3-dimensional tilings of a 2 X 2 X n box using 2 X 2 X 1
 *          plates and 1 X 2 X 1 dominos.
 * https://oeis.org/A359884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)+9*a(n-2)-14*a(n-3), with a(0)=1, a(1)=3, a(2)=24
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
#if defined(__cplusplus)
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;
    static size_t cap = 0;
#endif
    static idx_t computed_max = 0;

    /* Ensure capacity for indices 0..n (and at least up to 2) */
#if defined(__cplusplus)
    if (cache.size() < 3) cache.resize(3);
    if ((idx_t)cache.size() <= n) cache.resize(n + 1);
#else
    if (!cache || cap < 3) {
        size_t new_cap = 3;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }
    if (cap <= n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }
#endif

    /* Initialize base values once */
    if (computed_max < 2) {
#if defined(__cplusplus)
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)24;
#else
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)24;
#endif
        computed_max = 2;
    }

    /* Return if already computed */
    if (n <= computed_max)
        return cache[n];

    /* Compute new values using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        cache[i] = 5*a1 + 9*a2 - (seq_t)14*a3;
    }
    computed_max = n;
    return cache[n];
}
