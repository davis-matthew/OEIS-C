/*
 * A387870  a(n) = Sum_{k=0..n} binomial(3*n+1,4*k).
 * https://oeis.org/A387870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387870: a(n) = 4*a(n‑1) + 24*a(n‑2) + 64*a(n‑3),
   a(0) = 1, a(1) = 2, a(2) = 36.   */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Return the n-th term of sequence A387870 with memoisation. */
seq_t a(idx_t n)
{
    /* Expand cache if needed. */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache;

#if defined(__cplusplus)
        /* C++: use operator new/delete for proper construction. */
        new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        /* C: realloc works with a void* return value. */
        void *p = realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();
        new_cache = (seq_t *)p;
#endif

        /* Compute new terms. */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)1;
            else if (i == 1)
                new_cache[i] = (seq_t)2;
            else if (i == 2)
                new_cache[i] = (seq_t)36;
            else
                new_cache[i] = (seq_t)4 * new_cache[i - 1]
                            + (seq_t)24 * new_cache[i - 2]
                            + (seq_t)64 * new_cache[i - 3];
        }

        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[n];
}
