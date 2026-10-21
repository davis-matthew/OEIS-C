/*
 * A297621  a(0) = 1; for n > 0, a(n) = 1 + Sum_{k = 1..n} 2^k
 *          a(floor(log_2(k))).
 * https://oeis.org/A297621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + 2^n * a(floor(log2 n)) for n > 0
 *            (equivalently a(n) = 1 + Σ_{k=1..n} 2^k a(⌊log2 k⌋))
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 53, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline int floor_log2_u64(idx_t v) {
    int r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Static memoisation state */
static seq_t *cache      = NULL;   /* a(0)...a(last) */
static size_t cache_cap  = 0;      /* allocated size */
static idx_t  last       = (idx_t)-1;/* largest index computed */
static seq_t  pow2_last   = (seq_t)0;  /* 2^last */

seq_t a(idx_t n) {
    /* Initial allocation */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        last    = 0;
        pow2_last = (seq_t)1;
    }

    /* Grow array if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;          /* realloc failure */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Compute missing values up to n */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            pow2_last = pow2_last * (seq_t)2;                 /* 2^i */
            int fl = floor_log2_u64(i);                      /* floor(log2 i) */
            seq_t term = pow2_last * cache[fl];
            cache[i] = cache[i - 1] + term;
        }
        last = n;
    }

    return cache[n];
}
