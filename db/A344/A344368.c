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
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"

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
