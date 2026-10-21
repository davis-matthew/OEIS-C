/*
 * A298570  Number of n X 3 0..1 arrays with every element equal to 0, 2, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-3) + 2*a(n-4) + a(n-5) - 10*a(n-6) - 2*a(n-7) -
 *            2*a(n-8) + 2*a(n-9) Initial values (n = 1 … 9): a(1)=1, a(2)=4,
 *            a(3)=1, a(4)=2, a(5)=14, a(6)=10, a(7)=12, a(8)=72, a(9)=82
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 72, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A298570. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms, index 0 unused */
    static size_t cache_size = 0;      /* number of slots allocated (>= 2) */
    static size_t cache_max  = 0;      /* largest index that has been computed */

    /* Allocate at least as many slots as needed for the largest request */
    size_t needed = (size_t)n + 1; /* because we use 1‑based indexing */
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL)
            abort();            /* allocation failure */
        cache      = tmp;
        cache_size = needed;
    }

    /* Initialise base values (1…9) on first use or after a reset */
    if (cache_max < 9) {
        /* Ensure we have enough space for indices 0…9 */
        if (cache_size < 10) {
            seq_t *tmp = (seq_t *)realloc(cache, 10 * sizeof(seq_t));
            if (tmp == NULL)
                abort();
            cache      = tmp;
            cache_size = 10;
        }
        cache[0] = (seq_t)0;      /* unused slot */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)14;
        cache[6] = (seq_t)10;
        cache[7] = (seq_t)12;
        cache[8] = (seq_t)72;
        cache[9] = (seq_t)82;
        cache_max = 9;
    }

    /* If the requested index is already in the cache, just return it. */
    if ((size_t)n <= cache_max)
        return cache[(size_t)n];

    /* Compute missing terms up to n (inclusive) using the recurrence. */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)7 * cache[i - 3]
                   + (seq_t)2 * cache[i - 4]
                   + cache[i - 5]
                   - (seq_t)10 * cache[i - 6]
                   - (seq_t)2 * cache[i - 7]
                   - (seq_t)2 * cache[i - 8]
                   + (seq_t)2 * cache[i - 9];
        cache[i] = term;
    }
    cache_max = (size_t)n;

    return cache[(size_t)n];
}
