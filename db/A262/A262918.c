/*
 * A262918  Number of (4+1) X (n+1) 0..1 arrays with each row divisible by 3
 *          and each column divisible by 7, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 11*a(n-2) - 147*a(n-3) + 99*a(n-4) +
 *            777*a(n-5) - 1019*a(n-6) - 637*a(n-7) + 910*a(n-8), with a(1)=5,
 *            a(2)=9, a(3)=53, a(4)=137, a(5)=853, a(6)=2953, a(7)=17333,
 *            a(8)=71721
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial constants for n = 1 … 8. */
    static const seq_t init[8] = {
        (seq_t)5,   (seq_t)9,   (seq_t)53,   (seq_t)137,
        (seq_t)853, (seq_t)2953, (seq_t)17333, (seq_t)71721
    };

    /* Memoisation cache.
       cache[i] holds a(i+1).  cache_len is the number of terms stored. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Small indices: return constant table.  (The sequence is defined
       for n ≥ 1, but callers will not ask for n = 0.) */
    if (n <= 8)
        return init[(size_t)(n - 1)];

    /* Ensure the cache can hold at least n elements. */
    size_t need = (size_t)n;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* If cache is empty, fill the first 8 terms. */
    if (cache_len == 0) {
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }

    /* If the requested term is already computed, just return it. */
    if (n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n using the recurrence. */
    for (size_t k = cache_len + 1; k <= need; ++k) {
        seq_t val =
              ( 7 * cache[k - 2]
              + 11 * cache[k - 3]
              - 147 * cache[k - 4]
              + 99 * cache[k - 5]
              + 777 * cache[k - 6]
              - 1019 * cache[k - 7]
              - 637 * cache[k - 8]
              + 910 * cache[k - 9] );
        cache[k - 1] = val;
    }
    cache_len = need;
    return cache[(size_t)(n - 1)];
}
