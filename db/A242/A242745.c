/*
 * A242745  Least k > 0 such that (k!-n!)/(k-n) is an integer.
 * https://oeis.org/A242745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that (k! - n!)/(k - n) is an integer
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a_single.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n_cached = 0;   /* largest n for which cache[n] is set */

    if (n == 0) return 0;   /* undefined, return 0 */

    if (n > max_n_cached) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;
        for (idx_t cur = max_n_cached + 1; cur <= n; ++cur) {
            cache[cur] = a_single(cur);
        }
        max_n_cached = n;
    }
    return cache[n];
}
