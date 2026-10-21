/*
 * A291404  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S^2 - 2 S^4.
 * https://oeis.org/A291404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 * a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 4,
 * a(4) = 12, a(5) = 23, a(6) = 42, a(7) = 89, a(8) = 188
 * a(n) = a(n-2) + 2*a(n-3) + 3*a(n-4) + 8*a(n-5) + 12*a(n-6)
 *        + 8*a(n-7) + 2*a(n-8)   for n >= 9
 */

/* standard headers for both C and C++ */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

/* optional C++ header for new/delete */
#ifdef __cplusplus
# include <new>
#endif

/* initial values (length 9) */
static const seq_t init[9] = {0, 1, 2, 4, 12, 23, 42, 89, 188};

/* cache of computed terms */
static seq_t *cache = NULL;          /* array of terms up to current cached index */
static idx_t cached = 0;             /* highest index that has been computed */
static size_t cap = 0;               /* allocated capacity of cache */

/* ensure cache has space for indices up to n */
static void grow_cache(idx_t n)
{
    if (cap > (size_t)n) return;          /* already large enough */

    size_t new_cap = (cap == 0) ? 9 : cap * 2;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */

#ifdef __cplusplus
    /* C++: use new/delete for proper construction/destruction */
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    /* C: use realloc/malloc */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* fatal on allocation failure */
    cache = new_cache;
#endif

    cap = new_cap;
}

/* compute the n-th term of sequence A291404 */
seq_t a(idx_t n)
{
    /* return pre‑computed values directly for the first 9 indices */
    if (n < 9) return init[(int)n];

    /* initialize cache on first use */
    if (!cache) {
        /* allocate space for at least the first 9 terms */
#ifdef __cplusplus
        cache = new seq_t[9];
#else
        cache = (seq_t *)malloc(9 * sizeof(seq_t));
#endif
        if (!cache) abort();
        for (idx_t i = 0; i < 9; ++i) cache[i] = init[(int)i];
        cap = 9;
        cached = 8;                     /* indices 0..8 are known */
    }

    /* if the requested index is already known, return it */
    if (n <= cached) return cache[n];

    /* grow the cache so that it can hold up to index n */
    grow_cache(n);

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val = cache[i - 2];
        val += 2 * cache[i - 3];
        val += 3 * cache[i - 4];
        val += 8 * cache[i - 5];
        val += 12 * cache[i - 6];
        val += 8 * cache[i - 7];
        val += 2 * cache[i - 8];
        cache[i] = val;
    }

    cached = n;
    return cache[n];
}
