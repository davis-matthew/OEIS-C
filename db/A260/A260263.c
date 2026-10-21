/*
 * A260263  a(n+1) = a(n) + largest digit not in a(n), starting with a(1) = 1.
 * https://oeis.org/A260263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1,
     a(n+1) = a(n) + largest digit not appearing in a(n)   (for n ≥ 1) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"

/* cache of computed terms (1‑based indexing) */
static seq_t *cache = NULL;   /* array of terms */
static size_t capacity = 0;   /* allocated size of cache (elements) */
static size_t cached   = 0;   /* number of terms currently stored */

/* enlarge cache so that it can hold at least n terms (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= capacity)
        return;

    size_t needed = capacity ? capacity * 2 : n;
    if (needed < n)
        needed = n;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (tmp == NULL)
        exit(EXIT_FAILURE);   /* allocation failure */

    cache = tmp;
    capacity = needed;
}

/* compute a(n) for any 1‑based index n (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, but safe for callers */

    /* initialise cache on the first invocation */
    if (cached == 0) {
        ensure_cache(1);
        cache[0] = 1;          /* a(1) = 1 */
        cached = 1;
    }

    /* generate any missing terms up to n */
    if ((size_t)n > cached) {
        ensure_cache(n);
        for (size_t i = cached; i < (size_t)n; ++i) {
            seq_t prev = cache[i - 1];

            unsigned int freq[10] = {0};
            digit_frequencies(prev, freq);

            int largest_missing = 0;
            for (int d = 9; d >= 0; --d) {
                if (freq[d] == 0) {
                    largest_missing = d;
                    break;
                }
            }

            cache[i] = prev + (seq_t)largest_missing;
        }
        cached = (size_t)n;
    }

    return cache[n - 1];
}
