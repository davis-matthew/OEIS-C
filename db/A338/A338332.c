/*
 * A338332  Number of relatively prime 3-part integer partitions of n with no
 *          1's.
 * https://oeis.org/A338332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered integer partitions of n into three
 *            parts ≥ 2 whose greatest common divisor is 1
 * Range:     u8 n <= 58, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   len    = 0;

    /* initialise cache */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing entries */
    if (len <= n) {
        for (idx_t k = len; k <= n; ++k) {
            seq_t cnt = 0;

            for (idx_t a = 2; a <= k / 3; ++a) {
                idx_t max_b = (k - a) / 2;
                for (idx_t b = a; b <= max_b; ++b) {
                    idx_t c = k - a - b;

                    seq_t g = gcd((seq_t)a, (seq_t)b);
                    g = gcd(g, (seq_t)c);

                    if (g == (seq_t)1) {
                        ++cnt;
                    }
                }
            }
            cache[k] = cnt;
        }
        len = n + 1;
    }

    return cache[n];
}
