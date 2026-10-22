/*
 * A308445  a(0) = 1; a(n) = Sum_{k=1..n} gcd(n,k)*a(n-k).
 * https://oeis.org/A308445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} gcd(n,k)*a(n-k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  computed = 0;   /* largest index computed so far */
    if (cache == NULL) {
        /* initialise cache with a[0] = 1 */
        cap   = 1;
        cache = (seq_t*)seq_realloc(NULL, 0, cap);
        cache[0] = 1;
        computed = 0;
    }
    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t*)seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }
    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j < i; ++j) {
            seq_t g   = gcd((seq_t)i, (seq_t)j);
            sum += g * cache[j];
        }
        cache[i] = sum;
    }
    computed = n;
    return cache[n];
}
