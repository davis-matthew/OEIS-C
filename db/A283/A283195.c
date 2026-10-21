/*
 * A283195  Clone sequence C(2,3): a(1) = 1; for n > 1, a(n) is obtained by
 *          repeating each digit of a(n-1), summing the digits in groups of 3,
 *          and concatenating the digits of the sums.
 * https://oeis.org/A283195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n > 1, a(n) is obtained by cloning each digit of
 *            a(n‑1), summing the digits in groups of 3, and concatenating the
 *            sums
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   clone_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/clone_sum.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* a(1) is the first term */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Initialise cache if necessary */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(1) */
        cache_len = 1;
    }

    /* Expand cache up to n if required */
    if (n > cache_len) {
        cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        for (idx_t i = cache_len; i < n; ++i) {
            cache[i] = clone_sum(cache[i - 1]);
        }
        cache_len = n;
    }

    return cache[n - 1];
}
