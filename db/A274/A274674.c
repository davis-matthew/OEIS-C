/*
 * A274674  Diagonal of the rational function 1/(1 - x - x y - x z - y z + x y
 *          z).
 * https://oeis.org/A274674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,2k) * binomial(n+k,2k) *
 *            binomial(2k,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;        /* array of computed terms */
    static size_t cache_size = 0;      /* allocated size of cache */
    static idx_t computed_up_to = 0;   /* largest n that has been computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) {
            /* In the unlikely event of allocation failure, abort */
            abort();
        }
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        computed_up_to = 0;
    }

    /* Grow cache if necessary so that cache[n] is valid */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        /* It is safe to double the size to reduce realloc calls, but for simplicity
           we allocate exactly what is needed here. */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* If the requested term is already computed, return it immediately */
    if (n <= computed_up_to) {
        return cache[n];
    }

    /* Compute all terms from computed_up_to + 1 up to n */
    for (idx_t m = computed_up_to + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        idx_t maxk = m >> 1;                 /* floor(m/2) */
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t term;

            /* binomial(m, 2k) */
            term = binomial(m, k << 1);

            /* multiply by binomial(m + k, 2k) */
            term = term * binomial(m + k, k << 1);

            /* multiply by binomial(2k, k) */
            term = term * binomial(k << 1, k);

            sum += term;
        }
        cache[m] = sum;
    }

    computed_up_to = n;
    return cache[n];
}
