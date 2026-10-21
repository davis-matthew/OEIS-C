/*
 * A254682  Fifth partial sums of fifth powers (A000584).
 * https://oeis.org/A254682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Δ⁵ a(n) = n⁵ a(1)=1, a(2)=37, a(3)=418, a(4)=2754, a(5)=13080
 *            a(n) = 5*a(n-1) – 10*a(n-2) + 10*a(n-3) – 5*a(n-4) + a(n-5) + n⁵
 *            (n ≥ 6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 234, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle values below the offset */
    if (n == 0)
        return 0; /* behaviour below the offset does not matter */

    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    static const seq_t init[5] = {1, 37, 418, 2754, 13080};

    /* allocate initial cache */
    if (!cache) {
        cache_cap = 64;       /* start with room for 64 indices */
        cache_len  = 5;       /* we store indices 1..5 initially */
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_cap + 1)); /* +1 for index 0 unused */
        if (!cache) return 0; /* fallback on out‑of‑memory */
        /* store the first 5 terms */
        cache[1] = init[0];
        cache[2] = init[1];
        cache[3] = init[2];
        cache[4] = init[3];
        cache[5] = init[4];
    }

    /* grow cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) return 0;  /* fallback on out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t ni = (seq_t)i;
            /* compute n^5 */
            seq_t n2 = ni * ni;
            seq_t n3 = n2 * ni;
            seq_t n4 = n3 * ni;
            seq_t n5 = n4 * ni;

            /* recurrence: a(i) = 5*a(i-1) -10*a(i-2) +10*a(i-3) -5*a(i-4) +a(i-5) + n^5 */
            seq_t val = 5 * cache[i-1]
                       - 10 * cache[i-2]
                       + 10 * cache[i-3]
                       - 5  * cache[i-4]
                       +   cache[i-5]
                       +   n5;
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
