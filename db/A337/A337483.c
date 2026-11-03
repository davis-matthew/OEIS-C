/*
 * A337483  Number of ordered triples of positive integers summing to n that
 *          are either weakly increasing or weakly decreasing.
 * https://oeis.org/A337483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) for n>6
 * Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=4, a(6)=5
 */
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
    /* Handle indices 0..6 directly */
    if (n < 7) {
        switch (n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)1;
        case 4: return (seq_t)2;
        case 5: return (seq_t)4;
        case 6: return (seq_t)5;
        }
    }

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise the cache with the known first seven values */
    if (cache == NULL) {
        cache_len = 7;
        cache    = seq_alloc(cache_len);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)5;
    }

    /* Grow the cache if necessary and compute missing terms */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        cache = seq_realloc(cache, (size_t)old_len, (size_t)new_len);

        for (idx_t i = old_len; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val = val + cache[i - 2];
            val = val - cache[i - 4];
            val = val - cache[i - 5];
            val = val + cache[i - 6];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
