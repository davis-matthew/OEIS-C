/*
 * A252092  Numbers n such that the sum of the octagonal numbers N(n), N(n+1)
 *          and N(n+2) is equal to the pentagonal number P(m) for some m.
 * https://oeis.org/A252092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 940899*a(n-1) - 940899*a(n-2) + a(n-3) Initial values:
 *            a(1)=36, a(2)=34503186, a(3)=32463979328256
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index stored in cache */

    /*initialize cache with first three values*/
    if (cache == NULL) {
        /* we need space for indices 0..3 (index 0 unused) */
        const size_t init_size = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_size);
        if (!cache) return 0; /* Allocation failed, return 0 */

        cache[1] = (seq_t)36;
        cache[2] = (seq_t)34503186;
        cache[3] = (seq_t)32463979328256ULL;
        cache_len = 3;
    }

    /* Return immediately if already computed */
    if (n <= cache_len)
        return cache[n];

    /* Extend cache to accommodate up to n */
    if (cache_len < n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) return 0;   /* Allocation failed */

        /* compute missing terms */
        const seq_t MUL = 940899;
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            new_cache[k] = MUL * new_cache[k-1] - MUL * new_cache[k-2] + new_cache[k-3];
        }
        cache = new_cache;
        cache_len = n;
    }

    return cache[n];
}
