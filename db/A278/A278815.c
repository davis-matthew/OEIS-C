/*
 * A278815  Number of tilings of a 2 X n grid with monomers, dimers, and
 *          trimers.
 * https://oeis.org/A278815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + 5*a(n-3) - 2*a(n-4) - a(n-6), with
 *            initial terms: a(0)=1, a(1)=2, a(2)=7, a(3)=29, a(4)=109,
 *            a(5)=416
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    if (len == 0) {               /* initialize first six values */
        ensure_cache(6, &cache, &cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)29;
        cache[4] = (seq_t)109;
        cache[5] = (seq_t)416;
        len = 6;
    }

    if (n < len)
        return cache[(size_t)n];

    ensure_cache(n + 1, &cache, &cap);

    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t term = (seq_t)3 * cache[(size_t)(i - 1)];
        term += (seq_t)2 * cache[(size_t)(i - 2)];
        term += (seq_t)5 * cache[(size_t)(i - 3)];
        term -= (seq_t)2 * cache[(size_t)(i - 4)];
        term -= cache[(size_t)(i - 6)];
        cache[(size_t)i] = term;
    }

    len = (size_t)(n + 1);
    return cache[(size_t)n];
}
