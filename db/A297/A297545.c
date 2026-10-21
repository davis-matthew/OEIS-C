/*
 * A297545  Number of 2 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A297545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297545
   Recurrence: a(n) = 3*a(n-1) – 2*a(n-2) + 5*a(n-3) + 6*a(n-4) – 16*a(n-5) – 12*a(n-6)
   Initial values: a(1)=1, a(2)=2, a(3)=11, a(4)=24, a(5)=38, a(6)=105
   Non‑recursive implementation with memoization.  For C compilation
   (builtin integer seq_t) an unsigned __int128 intermediate is used to
   avoid overflow; for C++ compilation (arbitrary‑precision seq_t) the
   built‑in operator arithmetic is sufficient.  The function works for
   any order of calls. */

#include <stdlib.h>

#if defined(__cplusplus)
/* C++: use seq_t arithmetic directly */
static inline seq_t mul_const(seq_t x, unsigned int m) { return x * m; }
#else
/* C: use 128‑bit intermediate */
static inline seq_t mul_const(seq_t x, unsigned int m) {
    return (seq_t)(((__uint128_t)x) * m);
}
#endif

/* Cached values */
static seq_t *cache = NULL;          /* array holding computed a(i) values */
static size_t cache_cap = 0;         /* allocated size of cache (>= max needed + 1) */
static size_t cache_max = 0;         /* highest index computed so far */

/* Initial values for indices 1..6 (index 0 unused, set to 0) */
static const seq_t init[7] = {0, 1, 2, 11, 24, 38, 105};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* out of range, value irrelevant */

    if (n <= 6) return init[(size_t)n];

    /* Ensure cache is big enough */
    size_t needed = (size_t)n + 1;   /* include index 0 */
    if (!cache) {
        cache_cap = needed;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, unlikely */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        for (idx_t i = 1; i <= 6; ++i) cache[i] = init[i];
        cache_max = 6;
    } else if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return cache[cache_max]; /* fallback: return last known */
        cache = tmp;
        for (size_t i = cache_cap; i < needed; ++i) cache[i] = 0;
        cache_cap = needed;
    }

    /* Compute missing terms */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
#if defined(__cplusplus)
        /* C++ path – perform arithmetic with seq_t directly */
        seq_t left  = cache[i-1] * 3;
        left += cache[i-3] * 5;
        left += cache[i-4] * 6;

        seq_t right = cache[i-2] * 2;
        right += cache[i-5] * 16;
        right += cache[i-6] * 12;

        cache[i] = left - right;
#else
        /* C path – use unsigned __int128 to avoid intermediate overflow */
        __uint128_t left  = (__uint128_t)cache[i-1] * 3;
        left += (__uint128_t)cache[i-3] * 5;
        left += (__uint128_t)cache[i-4] * 6;

        __uint128_t right = (__uint128_t)cache[i-2] * 2;
        right += (__uint128_t)cache[i-5] * 16;
        right += (__uint128_t)cache[i-6] * 12;

        cache[i] = (seq_t)(left - right);
#endif
    }
    cache_max = (size_t)n;
    return cache[n];
}
