/*
 * A354784  First differences of A000213, also twice A000073.
 * https://oeis.org/A354784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 74, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A354784
 *  Formula: a(n) = a(n-1) + a(n-2) + a(n-3),  with
 *           a(0) = 0,  a(1) = 0,  a(2) = 2.  */
#include <stdlib.h>
#include <stddef.h>

/* Compute the nth term of A354784.  */
seq_t a(idx_t n)
{
    /* Static cache of already computed values.  */
    static seq_t *cache      = NULL;
    static size_t cap        = 0;      /* allocated size of cache   */
    static idx_t last        = 2;      /* largest index already
                                           initialized (initially 2)    */
    static int initialized   = 0;      /* 0 = not set up yet  */

    /* First call: create space for the first three values.  */
    if (!initialized) {
        /* Allocate at least three slots.  */
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* no allocation: abort with zero */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        initialized = 1;
    }

    /* Expand the cache if necessary.  */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap = new_cap ? new_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* realloc failed: keep old cache */
            return cache[(size_t)n];
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute values that have not yet been cached.  */
    if ((idx_t)n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[(size_t)i] =
                cache[(size_t)(i - 1)] +
                cache[(size_t)(i - 2)] +
                cache[(size_t)(i - 3)];
        }
        last = n;
    }

    return cache[(size_t)n];
}
