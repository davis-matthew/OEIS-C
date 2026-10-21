/*
 * A255140  a(1) = 1, a(n+1) = a(n)/gcd(a(n),n) if this gcd is > 1, else
 *          a(n+1) = a(n) + n + 2.
 * https://oeis.org/A255140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, let m = n-1, a(n) = a(m)/gcd(a(m),m) if
 *            gcd(a(m),m) > 1, otherwise a(n) = a(m) + m + 2
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache[0] unused, cache[1] = a(1) = 1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* last computed index */

    if (n < 1) return 0;             /* undefined for n = 0 */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0 and 1 */
        if (!cache) return 0;                               /* allocation failure */
        cache[1] = (seq_t)1;
        cache_size = 1;
    }

    /* extend cache up to n if necessary */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;                       /* allocation failure */
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            idx_t m   = i - 1;                  /* current n in recurrence */
            seq_t g   = gcd(prev, (seq_t)m);
            if (g > (seq_t)1) {
                cache[i] = prev / g;
            } else {
                cache[i] = prev + (seq_t)m + (seq_t)2;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
