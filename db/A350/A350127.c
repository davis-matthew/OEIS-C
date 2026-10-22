/*
 * A350127  a(n) = a(a(n-1)) mod 3 + a(n-2) with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A350127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(a(n-1)) mod 3 + a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 432, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;

    /* initialise first two terms */
    if (len == 0) {
        if (cap < 2) {
            cap = 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!cache) abort();
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* fast return if already computed */
    if (n < len)
        return cache[n];

    /* grow storage if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap : 2;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = len; i <= n; ++i) {
        /* a(a(i-1)) via memoised recursion */
        seq_t inner = a((idx_t)cache[i - 1]);
        seq_t inner_mod = inner % (seq_t)3;
        cache[i] = inner_mod + cache[i - 2];
    }
    len = n + 1;
    return cache[n];
}
