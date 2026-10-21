/*
 * A299506  a(n) = hypergeom([-n, n - 1/2], [1], -4).
 * https://oeis.org/A299506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = hypergeom([-n, n - 1/2], [1], -4) Recurrence: n*(2*n -
 *            3)*(4*n - 7)*a(n) = 9*(4*n - 5)*(4*n^2 - 10*n + 5)*a(n-1) -
 *            (n-1)*(2*n - 5)*(4*n - 3)*a(n-2) Initial values: a(0) = 1, a(1)
 *            = 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
    }

    /* Resize cache if it cannot hold up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_size) return cache[n];

    /* Initialise base cases if necessary */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1) {
        cache[1] = (seq_t)3;
        cache_size = 2;
    }

    /* Compute missing terms up to n */
    size_t nn = (size_t)n;
    for (size_t i = cache_size; i <= nn; ++i) {
        unsigned __int128 ii = (unsigned __int128)i;

        /* term1 = 9*(4*i-5)*(4*i^2-10*i+5)*a(i-1) */
        unsigned __int128 term1_factor = (unsigned __int128)9 *
                                         (4*ii - 5) *
                                         (4*ii*ii - (unsigned __int128)10*ii + 5);
        unsigned __int128 term1 = term1_factor * (unsigned __int128)cache[i-1];

        /* term2 = (i-1)*(2*i-5)*(4*i-3)*a(i-2) */
        unsigned __int128 term2_factor = (ii-1) *
                                         (2*ii - 5) *
                                         (4*ii - 3);
        unsigned __int128 term2 = term2_factor * (unsigned __int128)cache[i-2];

        unsigned __int128 numerator = term1 - term2;
        unsigned __int128 denom    = ii * (2*ii - 3) * (4*ii - 7);

        cache[i] = (seq_t)(numerator / denom);
    }
    cache_size = nn + 1;

    return cache[n];
}
