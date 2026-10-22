/*
 * A302393  Number of even parts in the partitions of 3n into 3 parts.
 * https://oeis.org/A302393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5) - a(n-6) +
 *            a(n-7) (n > 7) Initial values: a(1)=0, a(2)=5, a(3)=8, a(4)=18,
 *            a(5)=24, a(6)=41, a(7)=50
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache[0] unused, index 1..* fixed */
    static idx_t cache_size   = 0;       /* allocated size (indices 0..cache_size-1) */
    static idx_t computed_max = 0;       /* largest n for which cache[n] holds a value */

    if (n < 1)                     /* undefined for n<1, return 0 safely */
        return 0;

    /* Allocate or grow the cache */
    if (cache == NULL) {
        /* need at least indices 1..7 (offset 1) */
        cache_size = 8;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();

        /* initialise the first seven values */
        cache[1] = 0;
        cache[2] = 5;
        cache[3] = 8;
        cache[4] = 18;
        cache[5] = 24;
        cache[6] = 41;
        cache[7] = 50;
        computed_max = 7;
    } else if (cache_size <= n) {
        idx_t new_size = n + 1;          /* extend to include n */
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (n <= computed_max)
        return cache[n];

    /* Compute values from computed_max+1 up to n (recurrence) */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] - cache[i-3] + cache[i-4]
                 - cache[i-5] - cache[i-6] + cache[i-7];
    }

    computed_max = n;
    return cache[n];
}
