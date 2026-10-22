/*
 * A358682  Numbers k such that 8*k^2 + 8*k - 7 is a square.
 * https://oeis.org/A358682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) (n>3) Initial values:
 *            a(1)=1, a(2)=7, a(3)=43 This implementation memoises previously
 *            computed terms. It works for any seq_t (built‑in integer or
 *            arbitrary precision) and compiles as C11 and C++17
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i >= 1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index stored in cache */

    /* initialise cache with the first 3 terms on first use */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (cache == NULL) { return 0; }   /* allocation failure */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)43;
    }

    /* extend cache if a larger n is requested */
    if (n > cache_len) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == NULL) { return 0; }   /* allocation failure */

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = (seq_t)7 * cache[i - 1]
                     - (seq_t)7 * cache[i - 2]
                     + cache[i - 3];
        }
        cache_len = n;
    }

    return cache[n];
}
