/*
 * A250524  Number of (n+1)X(5+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 85*a(n-2) + 294*a(n-3) - 639*a(n-4) +
 *            906*a(n-5) - 839*a(n-6) + 490*a(n-7) - 164*a(n-8) + 24*a(n-9)
 *            initial values: a(1)=10660, a(2)=65310, a(3)=279161,
 *            a(4)=993123, a(5)=3183434, a(6)=9580060, a(7)=27710543,
 *            a(8)=78195145, a(9)=217483704
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 32, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms, index 0 unused */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* next free index (cache_len-1 is last computed) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* helper: ensure capacity for at least idx indices (0 .. idx-1) */
    if (cache_cap <= n + 1) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure, return 0 (not expected) */
        /* zero new part to avoid garbage */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise cache if not yet done */
    if (cache_len == 0) {
        /* indices 0 .. 9 are valid after this */
        cache_len = 10;
        cache[1] = (seq_t)10660;
        cache[2] = (seq_t)65310;
        cache[3] = (seq_t)279161;
        cache[4] = (seq_t)993123;
        cache[5] = (seq_t)3183434;
        cache[6] = (seq_t)9580060;
        cache[7] = (seq_t)27710543;
        cache[8] = (seq_t)78195145;
        cache[9] = (seq_t)217483704;
    }

    /* if requested term already cached, return it */
    if ((size_t)n < cache_len) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)14  * cache[i-1];
        val -= (seq_t)85  * cache[i-2];
        val += (seq_t)294  * cache[i-3];
        val -= (seq_t)639  * cache[i-4];
        val += (seq_t)906  * cache[i-5];
        val -= (seq_t)839  * cache[i-6];
        val += (seq_t)490  * cache[i-7];
        val -= (seq_t)164  * cache[i-8];
        val += (seq_t)24   * cache[i-9];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
