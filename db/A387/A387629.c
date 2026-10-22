/*
 * A387629  a(n) = Sum_{k=0..floor(n/4)} 2^k * binomial(2*n-6*k+1,2*k+1).
 * https://oeis.org/A387629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-4) + 4*a(n-5) - 4*a(n-8),
 *            initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=7, a(4)=11,
 *            a(5)=31, a(6)=83, a(7)=183
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 59, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {1, 3, 5, 7, 11, 31, 83, 183};

    /* static mutable cache ------------------------------------------------ */
    static seq_t *cache = NULL;          /* pointer to the cached values    */
    static idx_t  cached_n = 0;          /* maximum index currently cached  */
    static size_t cache_size = 0;        /* allocated array size in seq_t    */

    /* allocate and initialise cache on first call ------------------------- */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory – unlikely */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cached_n = 7;
    }

    /* if the requested term is already available, return it ---------------- */
    if (n <= cached_n)
        return cache[(size_t)n];

    /* enlarge cache if necessary ------------------------------------------- */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size;
        while ((size_t)(n + 1) > new_size)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;  /* out of memory – unlikely */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute terms from cached_n + 1 up to n using the recurrence -------- */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[(size_t)(i - 1)]
                  - cache[(size_t)(i - 2)]
                  + (seq_t)4 * cache[(size_t)(i - 4)]
                  + (seq_t)4 * cache[(size_t)(i - 5)]
                  - (seq_t)4 * cache[(size_t)(i - 8)];
        cache[(size_t)i] = val;
    }

    cached_n = n;
    return cache[(size_t)n];
}
