/*
 * A380820  a(0) = 0, a(1) = 1, and for n >= 2, a(n) = a(n-1) + a(n-2) if
 *          a(n-1) < n, otherwise a(n-1) - n.
 * https://oeis.org/A380820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, for n ≥ 2, * a(n) = a(n‑1) + a(n‑2) if
 *            a(n‑1) < n, otherwise a(n‑1) – n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cache for computed values.  The array grows geometrically on demand. */
static seq_t *cache      = NULL;
static bool *computed    = NULL;
static idx_t  cache_size = 0;

/* Make sure the arrays are large enough to hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = cache_size == 0 ? 2 : cache_size;
    while (new_size <= n)
        new_size <<= 1;

    seq_t *tmp  = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    bool *tmp2 = (bool *)realloc(computed, new_size * sizeof(bool));
    if (!tmp2) abort();
    computed = tmp2;

    /* Initialise new entries as "not computed".  The first two entries
     * are initialised to the known seed values when the cache grows
     * to contain them.
     */
    for (idx_t i = cache_size; i < new_size; ++i)
        computed[i] = false;

    /* Ensure the initial values are available if we have space for them. */
    if (new_size >= 2) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed[0] = true;
        computed[1] = true;
    }

    cache_size = new_size;
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);

    /* If the value has been computed before, we can return it
     * immediately.
     */
    if (computed[n])
        return cache[n];

    /* Compute all missing values up to n; the recurrence always
     * depends only on the two previous terms, so iterating forward
     * guarantees that cache[i-1] and cache[i-2] have already been
     * computed when we need them.
     */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (computed[i]) continue;

        seq_t prev      = cache[i - 1];
        seq_t prevprev  = cache[i - 2];
        seq_t result;

        if (prev < (seq_t)i)
            result = prev + prevprev;
        else
            result = prev - (seq_t)i;

        cache[i]     = result;
        computed[i]  = true;
    }

    return cache[n];
}
