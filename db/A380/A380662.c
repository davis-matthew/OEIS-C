/*
 * A380662  Numbers m such that Sum_{k>=0} floor(m/5^k) is prime.
 * https://oeis.org/A380662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m ≥ 1 for which * S(m) = Σ_{k≥0} ⌊m /
 *            5^k⌋ is a prime number
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple incremental search with a
 * cache of already found terms.  The sum S(m) is computed
 * as m + floor(m/5) + floor(m/25) + … . */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static inline seq_t trailing_sum(seq_t m)
{
    seq_t sum = m;
    while (m > 0) {
        m /= 5;
        sum += m;
    }
    return sum;
}

/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)                /* offset 1 (a(1) is first term) */
        return 0;

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* one‑time cache initialization */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache)
            return 0;          /* allocation failure – not expected */
    }

    /* grow the cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* start checking after the last found value */
        idx_t cur = (cache_size == 0) ? 1
                                      : (idx_t)(cache[cache_size - 1] + 1);

        /* find next m with S(m) prime */
        for (;;) {
            seq_t sum = trailing_sum((seq_t)cur);
            if (is_prime((idx_t)sum)) {
                /* enlarge cache if necessary */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap * 2 + 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp)
                        return 0;      /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)cur;
                break;              /* next term stored */
            }
            ++cur;
        }
    }
    return cache[n - 1];
}
