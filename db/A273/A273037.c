/*
 * A273037  Discriminator of the first n Fermat numbers A000215.
 * https://oeis.org/A273037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer k such that the first n
 *            Fermat numbers F_i = 2^(2^i)+1 (i = 0 … n-1) are pairwise
 *            incongruent modulo k
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   check_distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/check_distinct.h"

/* Main function requested by the OEIS kernel. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based cache of already computed values */
    static idx_t cache_len = 0;   /* current maximum index stored */

    if (n == 0) return 0; /* undefined, but safe guard */

    /* Grow cache if necessary */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure */
        cache = tmp;
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) cache[i] = 0;

        /* Compute missing values, starting from previous maximum. */
        for (idx_t m = cache_len + 1; m <= n; ++m) {
            seq_t start_k = (m == 1) ? (seq_t)1 : cache[m - 1];
            seq_t k = start_k;

            /* Search forward until distinctness holds. */
            while (true) {
                if (check_distinct(k, m)) {
                    cache[m] = k;
                    break;
                }
                ++k;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
