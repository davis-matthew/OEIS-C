/*
 * A242996  a(n) = (a(n-1)^2 - a(n-2)^4) * a(n-1) / a(n-2)^2 with a(1) = 1,
 *          a(2) = 2.
 * https://oeis.org/A242996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 6, i64 n <= 7, i128 n <= 8,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1, a(2) = 2
 * a(n) = (a(n-1)^2 - a(n-2)^4) * a(n-1) / a(n-2)^2        (n ≥ 3)
 *
 * The function memoises results so that calls in any order are efficient.
 */

#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;          /* array of computed terms, indexed by n */
static size_t cache_cap = 0;          /* current allocated capacity (≥ n+1)   */
static idx_t max_computed = 0;        /* largest n that has been computed    */

/* Return the n-th term of OEIS A242996 */
seq_t a(idx_t n)
{
    /* Handle first two known terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Make sure cache has room for index n (indices start at 1) */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise cache if this is the first call */
    if (max_computed < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        max_computed = 2;
    }

    /* If n is already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute subsequent terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t a_prev  = cache[i - 1];
        seq_t a_prev2 = cache[i - 2];

        /* a_prev2 squared and fourth powers */
        seq_t a_prev2_sq   = a_prev2 * a_prev2;          /* a_{i-2}^2   */
        seq_t a_prev2_quad = a_prev2_sq * a_prev2_sq;    /* a_{i-2}^4   */

        /* Numerator: a_prev^2 - a_prev2^4 */
        seq_t numerator = a_prev * a_prev - a_prev2_quad;

        /* Denominator: a_prev2^2 */
        seq_t denominator = a_prev2_sq;

        /* a_i = a_prev * numerator / denominator */
        cache[i] = a_prev * numerator / denominator;
    }

    max_computed = n;
    return cache[n];
}
