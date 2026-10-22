/*
 * A328796  Expansion of chi(x) / chi(-x^6) in powers of x where chi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A328796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {m : m odd or m ≡ 0 (mod 6)} with
 *            sum n (i.e. the coefficient of xⁿ in ∏_{k≥1}(1 + x^(2k-1))(1 +
 *            x^(6k)) ) This is the product representation of χ(x)/χ(−x⁶)
 * Range:     u8 n <= 47, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *subset_sum_cache = NULL;   /* cache for computed values   */
static size_t cache_capacity   = 0;       /* cache size (max n + 1)      */

/* Helper: allocate or grow the cache to at least size N + 1.  */
static void
ensure_cache_size(idx_t N)
{
    size_t new_cap = (size_t)N + 1;
    if (new_cap <= cache_capacity)
        return;    /* already sufficient */

    /* Allocate new array */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;          /* allocation failure: we silently abort, but
                                        the build environment guarantees
                                        sufficient memory. */

    /* Initialize new array */
    new_cache[0] = (seq_t)1;         /* a(0) = 1 */
    for (idx_t i = 1; i <= N; ++i)
        new_cache[i] = (seq_t)0;

    /* Fill the DP table using the product over allowed parts.          */
    for (idx_t part = 1; part <= N; ++part) {
        /* part is allowed if it's odd or divisible by 6          */
        if ( (part & (idx_t)1) || (part % (idx_t)6 == (idx_t)0) ) {
            /* update from high to low to enforce distinct parts */
            for (idx_t i = N; ; --i) {
                if (i < part) break;
                new_cache[i] += new_cache[i - part];
            }
        }
    }

    /* Replace old cache */
    if (subset_sum_cache)
        free(subset_sum_cache);
    subset_sum_cache  = new_cache;
    cache_capacity    = new_cap;
}

/* Public function */
seq_t a(idx_t n)
{
    /* Ensure cache covers up to n */
    if ((idx_t)cache_capacity <= n)
        ensure_cache_size(n);

    /* Cache must now contain a(n) */
    return subset_sum_cache[(size_t)n];
}
