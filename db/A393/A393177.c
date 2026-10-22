/*
 * A393177  Sum of the depths of symmetric valleys over all compositions of
 *          the integer n.
 * https://oeis.org/A393177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=2, for n ≥
 *            7: a(n) =
 *            2*a(n-1)+3*a(n-2)-3*a(n-3)-6*a(n-4)-3*a(n-5)+4*a(n-6)+4*a(n-7)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache previous values */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t cache_len = 0;   /* number of precomputed elements */

    /* enlarge cache if needed */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        }
        if (!cache) return (seq_t)0;  /* allocation failed: give up */

        /* initialise the new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            size_t ip = (size_t)i;            /* cast for array indexing */

            if (i <= 4) {
                cache[ip] = (seq_t)0;
            } else if (i == 5) {
                cache[ip] = (seq_t)1;
            } else if (i == 6) {
                cache[ip] = (seq_t)2;
            } else { /* i >= 7 */
                cache[ip] =
                    (seq_t)2 * cache[ip-1] + (seq_t)3 * cache[ip-2]
                    - (seq_t)3 * cache[ip-3] - (seq_t)6 * cache[ip-4]
                    - (seq_t)3 * cache[ip-5] + (seq_t)4 * cache[ip-6]
                    + (seq_t)4 * cache[ip-7];
            }
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
