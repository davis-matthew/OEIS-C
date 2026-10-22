/*
 * A384372  Numbers m such that both m-1 and m+1 are the product of at least 4
 *          not necessarily distinct primes.
 * https://oeis.org/A384372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer m such that bigomega(m-1) >= 4 and
 *            bigomega(m+1) >= 4
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

static seq_t *cached_terms = NULL;   /* Array of found terms */
static size_t cache_count = 0;        /* Number of terms cached so far */
static size_t cache_capacity = 0;     /* Allocated capacity of the array */
static idx_t last_checked = 1;        /* Last candidate m that was examined */

/* Grow the cache when more space is needed. */
static void grow_cache(void)
{
    size_t new_capacity = cache_capacity ? cache_capacity * 2 : 8;
    seq_t *new_terms = (seq_t *)malloc(sizeof(seq_t) * new_capacity);
    if (!new_terms) abort();                /* Memory allocation failed */
    if (cached_terms) {
        for (size_t i = 0; i < cache_count; ++i) new_terms[i] = cached_terms[i];
        free(cached_terms);
    }
    cached_terms = new_terms;
    cache_capacity = new_capacity;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe placeholder. */
    if (n == 0) return (seq_t)0;

    /* Find terms until we have at least n of them. */
    while (cache_count < (size_t)n) {
        ++last_checked;                /* Candidate m */

        /* m-1 and m+1 are safe because last_checked starts at 2. */
        idx_t m_minus = last_checked - 1;
        idx_t m_plus  = last_checked + 1;

        /* Check the omega condition using the supplied utility. */
        if (bigomega(m_minus) >= 4 && bigomega(m_plus) >= 4) {
            if (cache_count == cache_capacity) grow_cache();
            cached_terms[cache_count++] = (seq_t)last_checked;
        }
    }

    return cached_terms[(size_t)n - 1];   /* 1‑based index */
}
