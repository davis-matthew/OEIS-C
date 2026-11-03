/*
 * A265392  a(n) = denominator of Sum_{d|n} 1 / tau(d).
 * https://oeis.org/A265392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Σ_{d|n} 1 / τ(d)
 * Range:     u8 n <= 31, u16 n <= 4095, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 99 terms through n = 4428 (2026-11-03)
 * Helpers:   gcd, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  τ(d) = number of divisors of d.  The implementation
    iteratively adds the fractions 1/τ(d) and reduces by the
    greatest common divisor after each addition.  A simple
    cache is used so that repeated calls do not recompute
    the same value.  For n up to about 10 000 this runs well
    within the time limit.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0)        /* offset is 1; undefined for 0 */
        return 0;

    /* Ensure cache is large enough */
    if ((size_t)(n) > cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    if (cache[n - 1] != 0)
        return cache[n - 1];

    seq_t sum_num = 0;
    seq_t sum_den = 1;

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0)
            continue;

        seq_t tau = (seq_t)num_divisors(d);
        seq_t new_num = sum_num * tau + sum_den;
        seq_t new_den = sum_den * tau;

        seq_t g = gcd(new_num, new_den);
        if (g > 1) {
            new_num /= g;
            new_den /= g;
        }

        sum_num = new_num;
        sum_den = new_den;
    }

    cache[n - 1] = sum_den;
    return sum_den;
}
