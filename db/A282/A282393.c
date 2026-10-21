/*
 * A282393  Number of nX2 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors.
 * https://oeis.org/A282393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 3*a(n-1) + a(n-2) + 7*a(n-3) - 2*a(n-4) - 4*a(n-6)
   for n ≥ 7, with initial values:
   a(1)=4, a(2)=16, a(3)=57, a(4)=213, a(5)=796, a(6)=2964. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* current allocated size (highest index + 1) */
    static idx_t computed_max = 0; /* highest index whose value is computed */

    if (n == 0) return 0;          /* undefined for offset 1, but harmless */

    /* grow cache if needed */
    if (n >= cache_size) {
        size_t new_size = cache_size ? (size_t)cache_size * 2 : 64;
        while ((size_t)new_size <= n) new_size <<= 1;  /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) abort();        /* unrecoverable allocation failure */
        cache = tmp;
        cache_size = (idx_t) new_size;
    }

    /* initialise first values once */
    if (computed_max == 0) {
        cache[0] = (seq_t)0;      /* unused, but keeps indices simple */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)57;
        cache[4] = (seq_t)213;
        cache[5] = (seq_t)796;
        cache[6] = (seq_t)2964;
        computed_max = 6;
    }

    if (n <= computed_max)
        return cache[n];

    /* compute missing values up to n using the recurrence */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t val = 3 * cache[k - 1] + cache[k - 2] + 7 * cache[k - 3];
        val -= 2 * cache[k - 4];
        val -= 4 * cache[k - 6];
        cache[k] = val;
    }
    computed_max = n;
    return cache[n];
}
