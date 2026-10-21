/*
 * A284238  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 833", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 0 a(2) = 6 a(3) = 10 a(4) = 10 a(5) = 42 a(n) =
 *            a(n-1) + 4*a(n-2) - 4*a(n-3) for n > 5 (The sequence is computed
 *            by simple recurrence with memoisation.)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to support arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t len   = 0;  /* number of elements stored (next free index) */
    static size_t cap   = 0;  /* allocated capacity */

    /* helper: initialise the cache with the first terms */
    if (cache == NULL) {
        /* allocate at least 64 elements to minimise reallocs */
        cap = 64;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)10;
        cache[5] = (seq_t)42;
        len = 6;
    }

    /* resize if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values */
    if ((size_t)n + 1 > len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i < 6) continue; /* initial values already set */
            /* a(i) = a(i-1) + 4*a(i-2) - 4*a(i-3) */
            seq_t part1 = cache[i - 1];
            seq_t part2 = cache[i - 2] << 2; /* 4*a(i-2) */
            seq_t part3 = cache[i - 3] << 2; /* 4*a(i-3) */
            cache[i] = part1 + part2 - part3;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
