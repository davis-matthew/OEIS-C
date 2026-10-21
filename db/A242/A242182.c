/*
 * A242182  Numbers with four C's in Roman numerals.
 * https://oeis.org/A242182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less, base
 * Formula:   a(n) = a(n-1) + a(n-10) - a(n-11) for n > 11. Initial terms:
 *            a(1) = 390 a(2) = 391 a(3) = 392 a(4) = 393 a(5) = 394 a(6) =
 *            395 a(7) = 396 a(8) = 397 a(9) = 398 a(10) = 399 a(11) = 890
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of previously computed terms, 1‑based */
static idx_t  cached_up_to = 0; /* highest n for which cache[n] is valid */

/* Small table of the first 11 terms.  cache[0] is unused. */
static const seq_t first[12] = {
    0,          /* unused */
    390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 890
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* not part of the OEIS offset, safe guard */

    /* If we haven't computed up to n yet, extend the cache. */
    if (n > cached_up_to) {
        /* Allocate space for indices 0 .. n */
        size_t new_size = (size_t)(n + 1);   /* +1 for dummy 0 position */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0U;          /* allocation failed – return 0 */

        cache = new_cache;

        /* Compute missing entries.  The loop starts from cached_up_to+1
           (which may be 0) and goes up to n inclusive. */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            if (i <= 11) {                       /* use the hard‑coded table */
                cache[i] = first[(size_t)i];
            } else {                             /* recurrence for i > 11 */
                cache[i] = cache[i-1] + cache[i-10] - cache[i-11];
            }
        }
        cached_up_to = n;
    }

    return cache[n];
}
