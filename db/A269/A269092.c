/*
 * A269092  Number of n X 3 0..3 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1) - 378*a(n-2) + 972*a(n-3) - 729*a(n-4) for n>5
 *            with initial values: a(1)=24, a(2)=1152, a(3)=31296,
 *            a(4)=715320, a(5)=15024096
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;      /* array of seq_t: cache[i] = a(i+1) for i>=0 */
    static idx_t  cache_cap = 0;     /* current allocated capacity */
    static idx_t  cache_len = 0;     /* number of values currently stored */

    /* pre‑computed first five terms */
    static const seq_t first[5] = {
        (seq_t)24,
        (seq_t)1152,
        (seq_t)31296,
        (seq_t)715320,
        (seq_t)15024096
    };

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* arbitrary start size */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* increase capacity if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure: return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    while (cache_len < n) {
        idx_t idx = cache_len + 1;          /* 1‑based index of next value */
        seq_t val;
        if (idx <= 5) {
            val = first[cache_len];
        } else {
            seq_t pos = 36 * cache[cache_len - 1] + 972 * cache[cache_len - 3];
            seq_t neg = 378 * cache[cache_len - 2] + 729 * cache[cache_len - 4];
            val = pos - neg;                /* pos >= neg holds for all n>5 */
        }
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
