/*
 * A363712  Number of ways to write n as sum of a positive cube and a positive
 *          fourth power.
 * https://oeis.org/A363712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ways to write n as sum of a positive cube * and
 *            a positive fourth power * (OEIS A363712)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for the index below the OEIS offset. */
    if (n == 0)
        return (seq_t)0;

    /* Static state for memoization and a fast lookup table of fourth powers. */
    static seq_t *cache = NULL;          /* cache[i] holds a(i) for i <= computed */
    static unsigned char *isFour = NULL; /* isFour[x] == 1 iff x is a fourth power */
    static idx_t cache_size = 0;         /* capacity of cache array            */
    static idx_t bool_size = 0;          /* capacity of isFour array           */
    static idx_t computed = 0;           /* highest index for which cache[*] is set */

    /* Grow the cache array if needed (and initialise new entries to 0). */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Grow the isFour table if needed, and fill new entries. */
    if (n >= bool_size) {
        idx_t new_bool_size = n + 1;
        unsigned char *new_isFour = (unsigned char *)realloc(isFour,
                                                            new_bool_size * sizeof(unsigned char));
        if (!new_isFour) abort();
        /* initialise new entries to 0 */
        for (idx_t i = bool_size; i < new_bool_size; ++i)
            new_isFour[i] = 0;
        isFour = new_isFour;
        bool_size = new_bool_size;

        /* Mark all fourth powers up to the new maximum. */
        for (idx_t j = 1; ; ++j) {
            unsigned __int128 val128 = (unsigned __int128)j * j * j * j;
            if (val128 >= bool_size) break;
            idx_t val = (idx_t)val128;
            isFour[val] = 1;
        }
    }

    /* Compute results for all indices up to n that are not yet cached. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t count = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            unsigned __int128 cube128 = (unsigned __int128)k * k * k;
            if (cube128 >= i) break;
            idx_t cube = (idx_t)cube128;
            idx_t diff = i - cube;
            if (diff < bool_size && isFour[diff])
                ++count;
        }
        cache[i] = count;
    }

    computed = n;
    return cache[n];
}
