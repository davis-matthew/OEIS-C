/*
 * A351941  a(0) = 1; a(n) = -Sum_{k=0..floor((n-1)/2)} binomial(n-1,2*k) *
 *          a(k) * a(n-2*k-1).
 * https://oeis.org/A351941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0, a(n) = -∑_{k=0..⌊(n-1)/2⌋} binomial(n-1,2k) *
 *            a(k) * a(n-2k-1)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 18, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for binomial coefficients is assumed to be provided in the
   build environment as binomial(idx_t n, idx_t k).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;          /* memoized values a(i) */
static size_t cache_cap = 0;         /* current allocated size of cache */
static idx_t max_computed = 0;       /* highest index that has been computed */

/* Ensure that cache can hold indices up to n.  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0)
        abort();                     /* out of memory */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(i) for all i in (max_computed, n] and store them.  */
static void compute_upto(idx_t n)
{
    if (n == 0) {
        ensure_cache(0);
        cache[0] = 1;                 /* a(0) = 1 */
        max_computed = 0;
        return;
    }

    ensure_cache(n);
    cache[0] = 1;                     /* guarantee base case */

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t b = binomial(i - 1, 2 * k);
            seq_t prod = b * cache[k];
            prod = prod * cache[i - 2 * k - 1];
            sum = sum + prod;
        }
        cache[i] = -sum;
    }
    if (max_computed < n)
        max_computed = n;
}

/* Public entry point required by the harness.  */
seq_t a(idx_t n)
{
    compute_upto(n);
    return cache[n];
}
