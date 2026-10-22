/*
 * A323798  Number of permutations p of [n] such that max_{j=1..n} |p(j)-j| =
 *          2.
 * https://oeis.org/A323798
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-4) - 3*a(n-5) + a(n-6) + a(n-7)
 *            for n >= 8. Initial values for n < 8: a(0) = 0, a(1) = 0, a(2) =
 *            0, a(3) = 3, a(4) = 9, a(5) = 23, a(6) = 60, a(7) = 151. The
 *            sequence offset is 2: a(2) is the first term listed in OEIS
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* values for n = 0 and 1 are 0 */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size (>= next_index) */
    static idx_t next_index = 0;   /* first index not yet computed */

    /* Extend cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure fallback */
        cache_size = n + 1;
    } else if ((idx_t)(n + 1) > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure fallback */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute missing entries */
    for (idx_t i = next_index; i <= n; ++i) {
        seq_t val;
        if (i == 0 || i == 1 || i == 2)
            val = (seq_t)0;
        else if (i == 3)
            val = (seq_t)3;
        else if (i == 4)
            val = (seq_t)9;
        else if (i == 5)
            val = (seq_t)23;
        else if (i == 6)
            val = (seq_t)60;
        else if (i == 7)
            val = (seq_t)151;
        else {
            /* a(i) = 3*a(i-1) - a(i-2) - 2*a(i-4) - 3*a(i-5) + a(i-6) + a(i-7) */
            val = 3 * cache[i - 1];
            val = val - cache[i - 2];
            val = val - 2 * cache[i - 4];
            val = val - 3 * cache[i - 5];
            val = val + cache[i - 6];
            val = val + cache[i - 7];
        }
        cache[i] = val;
    }

    next_index = n + 1;
    return cache[n];
}
