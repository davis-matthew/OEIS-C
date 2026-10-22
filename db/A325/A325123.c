/*
 * A325123  Number of divisible pairs of positive integers up to n with no
 *          binary carries.
 * https://oeis.org/A325123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) if n is odd, a(n) = a(n-1) + Σ_{d|n, d<n, d & n ==
 *            0} 1 if n is even (precompute recursively over 0..n)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   count_divisors_no_carry
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_divisors_no_carry.h"

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* allocate if needed */
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        if (cache) free(cache);
        cache = tmp;
        cache_cap = new_cap;
        /* initialize first element */
        cache[0] = 0;
        last_computed = 0;
    }

    /* compute up to n if necessary */
    if (last_computed < n) {
        idx_t i;
        for (i = last_computed + 1; i <= n; ++i) {
            if ((i & 1) == 0) { /* even */
                seq_t add = count_divisors_no_carry(i);
                cache[i] = cache[i - 1] + add;
            } else { /* odd */
                cache[i] = cache[i - 1];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
