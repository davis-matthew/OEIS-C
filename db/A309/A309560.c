/*
 * A309560  a(1) = a(2) = 1, a(3) = 2, a(4) = 3, a(5) = 5, a(6) = a(7) = 6,
 *          a(8) = 7; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) +
 *          a(n-a(n-4)) for n > 8.
 * https://oeis.org/A309560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309560:
 * a(1)=1,a(2)=1,a(3)=2,a(4)=3,a(5)=5,a(6)=6,a(7)=6,a(8)=7;
 * a(n)=a(n-a(n-1))+a(n-a(n-2))+a(n-a(n-3))+a(n-a(n-4)) for n>8
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cached computed values.  cache[0] is unused; cache[i] == a(i). */
static seq_t *cache = NULL;
static size_t cap   = 0;   /* number of allocated slots (>= len+1) */
static size_t len   = 0;   /* highest n that has been computed so far */

/* Ensure the cache has at least required_size slots. */
static void grow_cache(size_t required_size)
{
    if (required_size <= cap)
        return;

    size_t new_cap = cap == 0 ? 16 : cap;
    while (new_cap < required_size)
        new_cap <<= 1;   /* double until large enough */

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_ptr == NULL)
        abort();        /* out of memory */

    cache = new_ptr;
    cap   = new_cap;
}

/* Initialise the first eight terms of the sequence. */
static void init_first_eight(void)
{
    if (cap < 9)           /* need indices 0..8 inclusive */
        grow_cache(9);

    cache[0] = 0;          /* dummy, not used */
    cache[1] = 1;          /* a(1) */
    cache[2] = 1;          /* a(2) */
    cache[3] = 2;          /* a(3) */
    cache[4] = 3;          /* a(4) */
    cache[5] = 5;          /* a(5) */
    cache[6] = 6;          /* a(6) */
    cache[7] = 6;          /* a(7) */
    cache[8] = 7;          /* a(8) */

    len = 8;
}

/* Return a(n) for any n >= 1.  Calls can arrive in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;     /* undefined for n = 0, but harmless */

    /* First call: set up the cache and base values. */
    if (cache == NULL)
        init_first_eight();

    /* Extend the cache and compute missing terms up to n. */
    if ((size_t)n > len) {
        grow_cache((size_t)n + 1);   /* +1 because we need index n itself */

        for (size_t i = len + 1; i <= (size_t)n; ++i) {
            if (i <= 8) {
                /* Base values already in cache; nothing to do. */
                continue;
            }

            /* All indices in the recurrence are guaranteed to be >= 1. */
            idx_t idx1 = (idx_t)i - (idx_t)cache[i - 1];
            idx_t idx2 = (idx_t)i - (idx_t)cache[i - 2];
            idx_t idx3 = (idx_t)i - (idx_t)cache[i - 3];
            idx_t idx4 = (idx_t)i - (idx_t)cache[i - 4];

            cache[i] =
                cache[idx1] + cache[idx2] + cache[idx3] + cache[idx4];
        }
        len = (size_t)n;
    }

    return cache[(size_t)n];
}
