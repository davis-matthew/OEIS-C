/*
 * A313747  Coordination sequence Gal.6.200.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4)-a(n-6)+a(n-7)+a(n-9)-a(n-10) for
 *            n>10 * Initial values:
 *            a(0)=1,a(1)=5,a(2)=10,a(3)=15,a(4)=20,a(5)=26,a(6)=31,a(7)=36,a(8)=42,a(9)=47,a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for 0 <= i <= last_computed */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = (idx_t)-1;   /* sentinel: not yet initialized */

    /* Initial values up to n = 10 */
    static const seq_t base[11] = {
        1, 5, 10, 15, 20, 26, 31, 36, 42, 47, 52
    };

    /* (Re)allocate cache to hold requested index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed – undefined result */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First use: fill the cache with the known initial values */
    if (last_computed == (idx_t)-1) {
        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = base[i];
        last_computed = 10;
    }

    /* Return if already computed */
    if (n <= last_computed)
        return cache[n];

    /* Compute values from last_computed+1 up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* The recurrence uses terms up to i-10, which exist for i>10 */
        seq_t pos = cache[i - 1];          /* + a(n-1) */
        pos += cache[i - 3];               /* + a(n-3) */
        pos += cache[i - 7];               /* + a(n-7) */
        pos += cache[i - 9];               /* + a(n-9) */

        seq_t neg = cache[i - 4];          /* - a(n-4) */
        neg += cache[i - 6];               /* - a(n-6) */
        neg += cache[i - 10];              /* - a(n-10) */

        cache[i] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
