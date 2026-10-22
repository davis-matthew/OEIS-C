/*
 * A360509  Number of words of length n over the alphabet [A-Z] that do not
 *          contain the string CAT.
 * https://oeis.org/A360509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=26, a(2)=26^2; for n≥3, a(n)=26*a(n-1)-a(n-3) (OEIS
 *            A360509)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;   /* dynamic cache for computed terms   */
static size_t cache_cap = 0;  /* allocated capacity                 */
static idx_t cache_last = 0;  /* highest index already computed     */

seq_t a(idx_t n)
{
    /* Handle the first three values without allocating the cache. */
    if (n < 3) {
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)26;
        return (seq_t)676;          /* 26*26 */
    }

    /* Ensure the cache has space for indices up to n. */
    if (cache_cap < (size_t)n + 1) {
        grow_seq(&cache, &cache_cap, (size_t)n + 1);
    }

    /* If the cache has not been initialised yet, do it now. */
    if (cache_cap == 0) {          /* Should not happen because of grow_seq above  */
        cache_cap = 3;
        cache = NULL;              /* In case grow_seq returned NULL */
        grow_seq(&cache, &cache_cap, 3);
    }

    /* Initialise the first three entries if they are not already set. */
    if (cache_cap >= 3 && cache_last < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)676;
        cache_last = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* Compute a(i) = 26*a(i-1) - a(i-3) */
        seq_t prod = cache[i-1] * (seq_t)26;
        cache[i] = prod - cache[i-3];
    }

    cache_last = n;
    return cache[n];
}
