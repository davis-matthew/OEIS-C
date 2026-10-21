/*
 * A263661  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -3 to 3 places and with no two consecutive
 *          increases or two consecutive decreases.
 * https://oeis.org/A263661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-4) for n>12 initial values: a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=10, a(5)=22, a(6)=40, a(7)=70, a(8)=133,
 *            a(9)=254, a(10)=482, a(11)=902, a(12)=1710
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 70, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization table (1‑based index) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index computed so far */

    if (n == 0) return 0;          /* undefined, return 0 as safe value */

    /* ensure the cache can hold indices up to n */
    if (cache == NULL) {
        /* allocate initial capacity */
        size_t cap = n + 1;
        if (cap < 16) cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache_size = 0;
    } else if (n >= cache_size && n + 1 > 0) {
        size_t needed = n + 1;
        if (needed > 0) {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!cache) return 0; /* allocation failed */
        }
    }

    /* base values (index 1 .. 12) */
    static const seq_t base[13] = {
        0,                /* base[0] unused */
        1, 2, 4, 10, 22,  /* a(1) .. a(5) */
        40, 70, 133, 254, /* a(6) .. a(9) */
        482, 902, 1710    /* a(10) .. a(12) */
    };

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 12)
            cache[i] = base[i];
        else
            cache[i] = 3 * cache[i - 2] + 2 * cache[i - 4];
    }
    cache_size = n;
    return cache[n];
}
