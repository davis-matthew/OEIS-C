/*
 * A321311  Number of linear chord diagrams having n+2 chords and minimal
 *          chord length n.
 * https://oeis.org/A321311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=10, a(2)=26, a(3)=79, a(4)=252, a(5)=796, a(n) = 5*a(n-1) -
 *            6*a(n-2) for n > 5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 952 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utility headers provide allocation helpers for seq_t */
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 safely */

    /* static cache holds previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms cached (a(1) at index 0) */

    /* ensure we have initial values cached */
    if (cache == NULL) {
        cache      = seq_alloc(5);
        cache_len  = 5;
        cache[0]   = (seq_t)10;
        cache[1]   = (seq_t)26;
        cache[2]   = (seq_t)79;
        cache[3]   = (seq_t)252;
        cache[4]   = (seq_t)796;
    }

    size_t target = (size_t)n;     /* target index (a(n) at cache[target-1]) */

    /* grow cache if necessary and compute new terms */
    if (target > cache_len) {
        cache = seq_realloc(cache, cache_len, target);
        for (size_t i = cache_len; i < target; ++i) {
            /* i = 0 corresponds to a(1), so i-1 -> a(i) and i-2 -> a(i-1) */
            seq_t val = 5 * cache[i - 1] - 6 * cache[i - 2];
            cache[i] = val;
        }
        cache_len = target;
    }

    return cache[target - 1];
}
