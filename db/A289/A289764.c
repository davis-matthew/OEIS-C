/*
 * A289764  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 597", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 4*a(n-4) - 6*a(n-5) +
 *            4*a(n-6) Initial values: a(0)=1, a(1)=2, a(2)=6, a(3)=13,
 *            a(4)=29, a(5)=58
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n) {
    /* initial constants for n < 6 */
    static const seq_t init[6] = {1, 2, 6, 13, 29, 58};

    if (n < 6) {
        return init[n];
    }

    /* static cache of previously computed terms */
    static idx_t cache_cap = 0;          /* allocated capacity */
    static idx_t cache_size = 0;         /* number of valid terms in cache */
    static seq_t *cache = NULL;          /* dynamic array of seq_t */

    /* initialize cache if first use */
    if (cache == NULL) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure; abort – not recoverable in this context */
            exit(EXIT_FAILURE);
        }
        for (idx_t i = 0; i < 6; ++i) {
            cache[i] = init[i];
        }
        cache_size = 6;
    }

    /* extend cache if needed */
    if (n >= cache_cap) {
        /* grow capacity geometrically to avoid repeated reallocations */
        while (cache_cap <= n) {
            cache_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) {
            exit(EXIT_FAILURE);
        }
        cache = tmp;
    }

    /* compute and store terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 3 * cache[i-1] - cache[i-2]
                   - 3 * cache[i-3] + 4 * cache[i-4]
                   - 6 * cache[i-5] + 4 * cache[i-6];
    }
    cache_size = n + 1;   /* we've computed up to index n */

    return cache[n];
}
