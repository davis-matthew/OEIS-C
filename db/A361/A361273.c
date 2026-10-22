/*
 * A361273  Number of 1324-avoiding even Grassmannian permutations of size n.
 * https://oeis.org/A361273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A361273 – 1324‑avoiding even Grassmannian permutations
 *
 *  G.f. : -(x^7+2*x^6-7*x^5-8*x^4+x^3+3*x^2-x-1)/((x+1)^4*(x-1)^4)
 *         = -(x^7+2*x^6-7*x^5-8*x^4+x^3+3*x^2-x-1)/(1-x^2)^4
 *
 *  Recurrence (for n ≥ 8):
 *      a(n) = 4*a(n-2) - 6*a(n-4) + 4*a(n-6) - a(n-8)
 *
 *  Initial values:
 *      a(0)=1, a(1)=1, a(2)=1, a(3)=3,
 *      a(4)=6, a(5)=13, a(6)=20, a(7)=37
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed values (lazy evaluated) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;            /* number of elements stored */

    /* initial eight values */
    static const seq_t init[8] = {
        1, 1, 1, 3, 6, 13, 20, 37
    };

    /* when first called, allocate space for the first 8 terms */
    if (cache == NULL)
    {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory, return 0 as sentinel */
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* extend cache if needed */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return 0; /* allocation failed */
        /* fill new entries using the recurrence */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            /* apply recurrence: a(i) = 4*a(i-2) - 6*a(i-4) + 4*a(i-6) - a(i-8) */
            seq_t term1 = 4 * new_cache[i - 2];
            seq_t term2 = 6 * new_cache[i - 4];
            seq_t term3 = 4 * new_cache[i - 6];
            seq_t term4 = new_cache[i - 8];
            new_cache[i] = term1 - term2 + term3 - term4;
        }
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
