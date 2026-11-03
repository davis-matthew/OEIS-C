/*
 * A307781  a(1) = 1; a(n+1) = -Sum_{d|n} a(d)^(n/d).
 * https://oeis.org/A307781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n+1) = -Sum_{d|n} a(d)^(n/d) (OEIS A307781)
 * Range:     i8 n <= 38, i16 n <= 74, i32 n <= 144, i64 n <= 284,
 *            i128 n <= 561, big any n
 * Validated: standard, pass, 76 terms through n = 9622 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
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
    /* trivial case */
    if (n == 0) return 0;                       /* undefined, safe fallback */

    /* static cache: cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache = NULL;
    static size_t cap = 0;                       /* current allocated capacity */
    static idx_t cur = 0;                       /* highest index that has been computed */

    /* make sure we have space for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* initialise first term the first time we need it */
    if (cur == 0) {
        cache[1] = 1;       /* a(1) = 1 */
        cur = 1;
    }

    /* compute missing terms up to n (if any) */
    for (idx_t i = cur + 1; i <= n; ++i) {
        /* we need a(i) = - Sum_{d | (i-1)} a(d)^( (i-1)/d ) */
        idx_t m = i - 1;          /* the n in the recurrence */
        seq_t sum = 0;

        /* iterate through all divisors d of m */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                /* divisor d */
                idx_t d1 = d;
                seq_t exp1 = (seq_t)(m / d1);
                seq_t term1 = ipow(cache[d1], exp1);
                sum += term1;

                /* paired divisor if distinct */
                idx_t d2 = m / d;
                if (d2 != d1) {
                    seq_t exp2 = (seq_t)(m / d2);
                    seq_t term2 = ipow(cache[d2], exp2);
                    sum += term2;
                }
            }
        }

        cache[i] = -sum;
    }

    cur = n;       /* record that we have computed up to n */
    return cache[n];
}
