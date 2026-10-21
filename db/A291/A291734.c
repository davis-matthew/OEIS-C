/*
 * A291734  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = (1 - S)(1 - 2 S).
 * https://oeis.org/A291734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 3*a(n-3) - 4*a(n-4) - 2*a(n-6);
 *            a(0)=3, a(1)=7, a(2)=18, a(3)=45, a(4)=108, a(5)=258, a(6)=615;
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial 7 terms */
static const seq_t _init_seq[7] = {3,7,18,45,108,258,615};

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (indices 0..cache_len-1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* handle the first 7 terms directly */
    if (n < 7)
        return _init_seq[(size_t)n];

    /* ensure cache is allocated and initialized */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* out of memory */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = _init_seq[i];
        cache_len = 7;
    }

    /* enlarge cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t pos = 3 * cache[i-1] + 3 * cache[i-3];
        seq_t neg = 2 * cache[i-2] + 4 * cache[i-4] + 2 * cache[i-6];
        cache[i] = pos - neg;   /* pos >= neg for all n >= 7 */
    }
    cache_len = need;

    return cache[(size_t)n];
}
