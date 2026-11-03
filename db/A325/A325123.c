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
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the number of proper divisors d of n such that the binary
   representations of d and n have no overlapping '1' bits. */
static seq_t count_divisors_no_carry(idx_t n)
{
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* d is a divisor */
            if (d < n && ( (d & n) == 0 ))
                ++sum;
            /* n/d is the paired divisor */
            idx_t d2 = n / d;
            if (d2 != d && d2 < n && ( (d2 & n) == 0 ))
                ++sum;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* allocate if needed */
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = mem_new(seq_t, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        if (cache) mem_free(cache);
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
