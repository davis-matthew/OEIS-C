/*
 * A247050  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,1) to (n,k), where 0 <= k <= 2, consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=0, a(5)=1, a(6)=0, a(7)=2,
 *            a(8)=1; a(n)=2*a(n-6)+a(n-9) for n>8
 * Range:     u8 n <= 42, u16 n <= 75, u32 n <= 144, u64 n <= 282,
 *            u128 n <= 561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t base_vals[9] = {0,1,0,1,0,1,0,2,1};

seq_t a(idx_t n)
{
    /* Number of cached elements (n+1 rounded up). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Expand the cache if necessary. */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {                /* allocation failure: return 0. */
            return (seq_t)0;
        }
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 9) {
                cache[i] = base_vals[i];
            } else {
                cache[i] = (seq_t)2 * cache[i-6] + cache[i-9];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
