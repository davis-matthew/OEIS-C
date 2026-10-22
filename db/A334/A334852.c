/*
 * A334852  a(1) = 1, a(n) = a(n-1) / gcd(a(n-1),n) if this gcd is > 1, else
 *          a(n) = a(n-1) + 2.
 * https://oeis.org/A334852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, for n>1 let g = gcd(a(n-1), n); if g > 1 then a(n) =
 *            a(n-1)/g else a(n) = a(n-1)+2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises previous values so that calls may be made in any order. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0;          /* n<1 is outside the OEIS offset; return 0 for safety */

    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);        /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 2];
                seq_t g = seq_gcd(prev, (seq_t)i);
                if (g > 1)
                    cache[i - 1] = prev / g;
                else
                    cache[i - 1] = prev + 2;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
