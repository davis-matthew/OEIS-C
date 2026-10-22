/*
 * A364370  Number of chordless cycles (of length > 3) in the complement of
 *          the n-hypercube graph.
 * https://oeis.org/A364370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 168*a(n-2) + 768*a(n-3) - 2064*a(n-4) +
 *            3264*a(n-5) - 2816*a(n-6) + 1024*a(n-7) Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=6, a(4)=160, a(5)=1720, a(6)=13056,
 *            a(7)=82656
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* allocated cache for computed terms */
static size_t cache_cap = 0;     /* current allocated capacity */
static idx_t  cache_len = 0;     /* number of terms already initialized */

/* Ensure cache has space for indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = needed;
        void *p = realloc(cache, new_cap * sizeof(seq_t));
        if (!p) {
            /* Allocation failed – abort */
            abort();
        }
        cache = (seq_t *)p;
        cache_cap = new_cap;
    }
}

/* Compute terms up to n, using the recurrence */
static void compute_up_to(idx_t n)
{
    /* Initial values for indices 0..7 */
    const seq_t init[8] = {
        (seq_t)0,  /* a(0) */
        (seq_t)0,  /* a(1) */
        (seq_t)0,  /* a(2) */
        (seq_t)6,  /* a(3) */
        (seq_t)160,/* a(4) */
        (seq_t)1720,/* a(5) */
        (seq_t)13056,/* a(6) */
        (seq_t)82656 /* a(7) */
    };

    idx_t i;
    for (i = cache_len; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init[(size_t)i];
        } else {
            seq_t v = 20 * cache[i-1];
            v -= 168 * cache[i-2];
            v += 768 * cache[i-3];
            v -= 2064 * cache[i-4];
            v += 3264 * cache[i-5];
            v -= 2816 * cache[i-6];
            v += 1024 * cache[i-7];
            cache[i] = v;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    if (cache_len <= n) {
        compute_up_to(n);
    }
    return cache[n];
}
