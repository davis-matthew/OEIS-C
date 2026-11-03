/*
 * A262677  Number of odd numbers encountered when iterating A049820 starting
 *          from n: a(0) = 0 and for n >= 1, a(n) = A000035(n) +
 *          a(A049820(n)).
 * https://oeis.org/A262677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = is_odd(n) + a(n - d(n)), a(0) = 0 where d(n) =
 *            divisor_count_u64(n) (OEIS A000005). The algorithm builds the
 *            values iteratively from 0 up to the requested n, using a static
 *            cache that is expanded on demand
 * Range:     u8 n <= 3496, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 124 terms through n = 9969 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include <stdlib.h>
#include <stdint.h>
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
    /* static cache: tmp[i] holds a(i) for i <= cache_max   */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_max  = 0;

    /* make sure we have room for indices 0..n   */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise a(0) – harmless to assign each time   */
    cache[0] = 0;

    /* compute values that have not yet been cached   */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        /* n - d(n)  (always < i for i >= 1)  */
        idx_t step = (idx_t)num_divisors(i);
        idx_t next = i - step;          /* guaranteed non‑negative           */

        /* a(i) = [i odd] + a(next)     */
        cache[i] = ((i & 1) ? (seq_t)1 : (seq_t)0) + cache[next];
    }

    cache_max = n;
    return cache[n];
}
