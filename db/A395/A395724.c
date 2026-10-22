/*
 * A395724  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} (-1)^k *
 *          binomial(n,2*k+1)^2 * a(n-2*k-1).
 * https://oeis.org/A395724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395724.
   Recurrence:
   a(0) = 1
   a(n) = Σ_{k=0..⌊(n-1)/2⌋} (-1)^k * binomial(n,2k+1)^2 * a(n-2k-1)
   (indices start at 0)
   This implementation memoizes values and computes binomial(n,2k+1)
   incrementally to avoid expensive repeated factorial calculations.
*/
#include <stddef.h>     /* size_t */
#include <stdbool.h>    /* bool */
#include "../utils/ensure_cache.h"   /* ensure_cache */

static seq_t *cache = NULL;        /* memoization table */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t max_computed = 0;     /* highest index computed */
static bool cache_initialized = false;

seq_t a(idx_t n)
{
    /* initialise memoisation table with a(0) */
    if (!cache_initialized) {
        ensure_cache(0, &cache, &cache_cap);     /* at least one slot */
        cache[0] = 1;
        max_computed = 0;
        cache_initialized = true;
    }

    /* already known value */
    if (n <= max_computed)
        return cache[n];

    /* extend table if necessary */
    ensure_cache(n, &cache, &cache_cap);

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = 0;                     /* sum of positive terms   */
        seq_t neg = 0;                     /* sum of negative terms   */
        bool even = true;                  /* (-1)^k : k even => +1   */

        /* iterate over k via odd j = 2k+1 */
        seq_t comb = i;                    /* binomial(i,1) = i        */
        for (idx_t j = 1; j <= i; j += 2) {
            /* term = comb^2 * a[i-j] */
            seq_t term = comb * comb;
            term = term * cache[i - j];

            if (even) pos += term; else neg += term;
            even = !even;                  /* toggle sign for next k */

            /* compute next binomial coefficient for odd j+2 */
            if (j + 2 <= i) {
                /* C(i, j+2) = C(i, j) * (i-j) * (i-j-1) / ((j+1)*(j+2)) */
                seq_t num = comb * ((seq_t)(i - j));
                num = num * ((seq_t)(i - j - 1));
                seq_t den = ( (seq_t)(j + 1) ) * ( (seq_t)(j + 2) );
                comb = num / den;
            }
        }

        /* result is positive */
        cache[i] = (pos >= neg) ? (pos - neg) : 0;
    }

    max_computed = n;
    return cache[n];
}
