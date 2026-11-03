/*
 * A371758  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-3*k-1,n-3*k).
 * https://oeis.org/A371758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-3*k-1 , n-3*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of allocated entries */

    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) exit(1);                /* allocation failed */
        cache = tmp;

        /* compute new values from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
                continue;
            }
            seq_t sum = 0;
            idx_t maxk = i / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t a = 2 * i - 3 * k - 1;
                idx_t b = i - 3 * k;
                sum += binomial(a, b);
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
