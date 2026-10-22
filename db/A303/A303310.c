/*
 * A303310  Number of nX4 0..1 arrays with every element equal to 0, 1, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+2*a(n-4)-a(n-5)-2*a(n-6)-a(n-7)
 *            Initial values: a(1)=5, a(2)=5, a(3)=17, a(4)=36, a(5)=74,
 *            a(6)=165, a(7)=361, a(8)=783, a(9)=1710, a(10)=3744
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, guard against misuse */

    static seq_t *cache = NULL;      /* stored terms a(1)..a(k) */
    static size_t cache_size = 0;    /* number of terms stored          */
    static size_t cache_cap  = 0;    /* allocated capacity               */

    /* Ensure the cache can hold n terms. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        if (new_cap < 16) new_cap = 16;          /* minimal capacity */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First time: initialise the first ten terms. */
    if (cache_size == 0) {
        cache[0] = 5;   cache[1] = 5;   cache[2] = 17;   cache[3] = 36;
        cache[4] = 74;  cache[5] = 165;  cache[6] = 361;  cache[7] = 783;
        cache[8] = 1710; cache[9] = 3744;
        cache_size = 10;
    }

    /* If the requested term is already computed, return it. */
    if ((size_t)n <= cache_size) return cache[n-1];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        size_t k = i - 1; /* array index for a(i) */
        seq_t val = cache[k-1] + cache[k-2] + 3 * cache[k-3] + 2 * cache[k-4]
                    - cache[k-5] - 2 * cache[k-6] - cache[k-7];
        cache[k] = val;
    }
    cache_size = n;
    return cache[n-1];
}
