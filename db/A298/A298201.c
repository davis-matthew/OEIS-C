/*
 * A298201  Number of Eulerian orientations of the torus grid graph C_4 X C_n.
 * https://oeis.org/A298201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 77*a(n-2) + 161*a(n-3) - 138*a(n-4) +
 *            40*a(n-5); a(1) = 32, a(2) = 114, a(3) = 548, a(4) = 2970, a(5)
 *            = 16892
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Internal cache for memoisation ------------------------------------------------- */
static seq_t *cache = NULL;          /* array of computed values (index 0 unused)   */
static size_t last_computed = 0;     /* highest n for which cache[n] is valid      */

/* ------------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (outside the defined offset) */
    if (n == 0)
        return (seq_t) 0;

    /* Initialise cache on first call.  Cache indices 1..5 contain the initial
       values of the sequence.  cache[0] is unused but set to 0 for safety.     */
    if (cache == NULL) {
        size_t init_size = 6;        /* indices 0 .. 5 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL)          /* allocation failure – abort */
            exit(1);

        /* initialise the first five terms (index 1..5) */
        const seq_t init[6] = { (seq_t)0,
                                (seq_t)32,
                                (seq_t)114,
                                (seq_t)548,
                                (seq_t)2970,
                                (seq_t)16892 };

        for (size_t i = 0; i < init_size; ++i)
            cache[i] = init[i];

        last_computed = 5;
    }

    /* If the requested term is already cached, return it immediately. */
    if ((size_t)n <= last_computed)
        return cache[n];

    /* Extend the cache if necessary.  We allocate up to index n inclusive. */
    size_t needed = (size_t)n + 1;   /* array size needed (0..n) */
    if (needed > last_computed + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (new_cache == NULL)        /* reallocation failed – abort */
            exit(1);
        cache = new_cache;
    }

    /* Compute the missing terms sequentially using the recurrence. */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 5) {
            /* Should not happen because we already initialised 1..5,
               but keep this guard for safety.                       */
            const seq_t init[6] = { (seq_t)0,
                                    (seq_t)32,
                                    (seq_t)114,
                                    (seq_t)548,
                                    (seq_t)2970,
                                    (seq_t)16892 };
            cache[i] = init[i];
            continue;
        }

        /* Compute a(i) from the linear recurrence:
           a(i) = 15*a(i-1) - 77*a(i-2) + 161*a(i-3) - 138*a(i-4) + 40*a(i-5)
        */
        seq_t positive = 15 * cache[i-1] + 161 * cache[i-3] + 40 * cache[i-5];
        seq_t negative = 77 * cache[i-2] + 138 * cache[i-4];
        cache[i] = positive - negative;   /* safe because positive >= negative */
    }

    last_computed = (size_t)n;
    return cache[n];
}
