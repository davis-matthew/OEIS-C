/*
 * A248151  Numbers n such that the sum of the divisors of n is not divisible
 *          by 4.
 * https://oeis.org/A248151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 937 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k such that sigma_of(k) % 4 ≠ 0 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t *cache = NULL;           /* stored a(i) values           */
static size_t cache_len = 0;          /* number of cached terms       */
static size_t cache_cap = 0;          /* current cache capacity       */
static idx_t  next_candidate = 1;     /* next integer to test         */

/* Return true iff σ(n) mod 4 ≠ 0.  Uses the provided sigma_of(). */
static bool sigma_not_div_by_4(idx_t n)
{
    seq_t s = sigma_of(n);
    return (s % (seq_t)4) != 0;
}

/* Return the nth term of A248151.  The function builds a cache
   lazily, so any order of calls is supported.  It works for
   all allowed seq_t/idx_t types. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; out of range */
        return 0;

    while (cache_len < n) {
        /* Find next integer whose divisor sum is not divisible by 4 */
        while (!sigma_not_div_by_4(next_candidate))
            ++next_candidate;

        /* Ensure the cache has room for the new value */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;   /* increment to the next candidate for the next round */
    }

    return cache[n - 1];
}
