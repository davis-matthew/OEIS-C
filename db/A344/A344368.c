/*
 * A344368  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(1-s)).
 * https://oeis.org/A344368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A001055(n) (A001055(n) = number of unordered
 *            factorizations of n>1, including the trivial factorization n)
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* Recursive helper: count unordered factorizations of n with minimal factor >= min_f */
static seq_t
count_factorizations(idx_t n, idx_t min_f)
{
    /* The trivial factorization n itself */
    seq_t res = 1;

    /* Enumerate divisors d >= min_f with d*d <= n */
    for (idx_t d = min_f; d * d <= n; ++d) {
        if (n % d == 0) {
            /* Count factorizations of the cofactor n/d using d as the new minimal factor */
            res += count_factorizations(n / d, d);
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0; return 0 */
        return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Ensure storage for index n-1 */
    ensure_cache(n, &cache, &cap);

    /* Return cached value if already computed */
    if (cache[n - 1] != 0)
        return cache[n - 1];

    seq_t result;
    if (n == 1)
        result = 1;
    else {
        seq_t count = count_factorizations(n, 2);
        result = (seq_t)n * count;
    }

    cache[n - 1] = result;
    return result;
}
