/*
 * A292187  Number of rooted unlabeled bipartite cubic maps on a compact
 *          closed oriented surface with 2*n vertices (and thus 3*n edges),
 *          with a(0) = 1.
 * https://oeis.org/A292187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A292187):
   a(0) = 1
   a(1) = 2
   a(n) = 3*n*a(n-1) + Σ_{k=1}^{n-2} a(k)*a(n-k-1)   (n ≥ 2)
   The function below memoises results so that calls in any order
   are fast and correct.
*/
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache        = NULL;   /* array holding a(i) */
static idx_t cache_len    = 0;        /* allocated size (highest index + 1) */
static idx_t computed_high = 0;       /* highest index that has been computed */

/* Ensure that cache can hold indices up to n (inclusive).  If cache is
   newly allocated, initialise a(0) and a(1).  When cache already exists,
   the previous values are preserved. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_len)
        return;               /* already large enough */

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;               /* out of memory – caller will fail gracefully */

    /* copy existing values */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* zero the new entries */
    for (idx_t i = cache_len; i < new_cap; ++i)
        new_cache[i] = 0;

    free(cache);
    cache = new_cache;
    cache_len = new_cap;

    /* initialise base values if this is the first allocation */
    if (cache_len >= 2 && computed_high < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        computed_high = 1;
    }
}

/* Compute all a(i) up to limit (inclusive) using the recurrence,
   assuming cache is large enough. */
static void compute_up_to(idx_t limit)
{
    if (limit <= computed_high)
        return;               /* already done */

    for (idx_t n = computed_high + 1; n <= limit; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= n - 2; ++k)
            sum += cache[k] * cache[n - k - 1];
        cache[n] = (seq_t)3 * (seq_t)n * cache[n - 1] + sum;
    }
    computed_high = limit;
}

/* Public entry point: returns a(n).  Handles n = 0,1 directly. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    ensure_capacity(n);        /* make sure we have room */
    compute_up_to(n);          /* compute missing values */
    return cache[n];
}
