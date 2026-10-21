/*
 * A298791  Partial sums of A298789.
 * https://oeis.org/A298791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + 2*a(n‑3) – 2*a(n‑4) – a(n‑6) + a(n‑7) (for n>6)
 *            with initial values a(0)=1, a(1)=5, a(2)=12, a(3)=22, a(4)=37,
 *            a(5)=55, a(6)=75
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of allocated entries   */
    static idx_t  max_cached = 0;     /* highest n already computed    */

    /* initial 7 terms */
    static const seq_t init[7] = {1, 5, 12, 22, 37, 55, 75};

    /* return an initial term directly */
    if (n <= 6)
        return init[n];

    /* first use: allocate cache and copy the initial terms */
    if (cache == NULL) {
        cache_len = 7;                          /* indices 0…6 ready      */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return 0;                           /* allocation failure      */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        max_cached = 6;
    }

    /* if we already have a(n), just return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* enlarge the cache to hold up to n */
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0;                           /* allocation failure      */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing values up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += 2 * cache[i - 3];
        val -= 2 * cache[i - 4];
        val -= cache[i - 6];
        val += cache[i - 7];
        cache[i] = val;
    }
    max_cached = n;

    return cache[n];
}
