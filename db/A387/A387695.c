/*
 * A387695  a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) *
 *          binomial(2*n-4*k+1,2*k+1).
 * https://oeis.org/A387695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 1425 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387695
   Formula (by summation): a(n) = Sum_{k=0..floor(n/3)} 2^(n-3k) * binomial(2n-4k+1,2k+1)
   Recurrence (for n≥6): a(n) = 4 a(n-1) – 4 a(n-2) + 2 a(n-3) + 4 a(n-4) – a(n-6)
   Base values: a(0)=1, a(1)=6, a(2)=20, a(3)=57, a(4)=164, a(5)=492
   Index: a(0) is the first term. */

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

static seq_t *cache      = NULL; /* dynamically growing cache */
static idx_t  cache_len  = 0;     /* number of computed terms          */
static size_t cache_cap  = 0;     /* allocated size (in terms)         */

static const seq_t FOUR = (seq_t)4;
static const seq_t TWO  = (seq_t)2;

/* Initialise cache with the first six terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;                     /* already initialised */

    cache_cap = 16;                 /* arbitrary small start */
    cache     = seq_alloc(cache_cap);
    if (!cache)
        abort();

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)20;
    cache[3] = (seq_t)57;
    cache[4] = (seq_t)164;
    cache[5] = (seq_t)492;
    cache_len = 6;
}

/* Make sure terms up to n are computed and cached. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;               /* already available */

    init_cache();

    /* Grow cache if necessary. */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = ((size_t)(n + 1)) * 2;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp)
            abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v;
        if (i >= 6) {
            /* a(i) = 4*a(i-1) – 4*a(i-2) + 2*a(i-3) + 4*a(i-4) – a(i-6) */
            v = FOUR * cache[i - 1];
            v -= FOUR * cache[i - 2];
            v += TWO  * cache[i - 3];
            v += FOUR * cache[i - 4];
            v -=     cache[i - 6];
        } else {
            /* Base cases already set; skip recomputing. */
            v = cache[i];
        }
        cache[i] = v;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
