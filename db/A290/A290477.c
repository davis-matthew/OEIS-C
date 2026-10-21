/*
 * A290477  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,4,1,5 (the first five digits of
 *          Pi).
 * https://oeis.org/A290477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n>1 : a(n)=6*a(n-1)+pattern[(n-1) mod 5]   */
/* pattern = {3,1,4,1,5}.  The values are the decimal values of the base‑6 */
/* numbers formed by the repeating digit sequence 3,1,4,1,5. */
#include <stdlib.h>
#include <stdint.h>

/* Memoisation cache for previously computed terms. */
static seq_t *cache      = NULL; /* pointer to array a[0..cached_n-1] */
static idx_t  cached_n   = 0;    /* number of terms currently cached */

/* The repeating base‑6 digit pattern. */
static const seq_t pattern[5] = { (seq_t)3, (seq_t)1, (seq_t)4, (seq_t)1, (seq_t)5 };

/* Return the n‑th term of sequence A290477 (1‑based index). */
seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1 only. */
    if (n < 1) return (seq_t)0;

    /* If the requested term is already cached, return it directly. */
    if (cached_n >= n)
        return cache[n - 1];

    /* Resize the cache if necessary. */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
    } else if (cached_n < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
    }

    /* Compute missing terms up to n. */
    seq_t last = (cached_n > 0) ? cache[cached_n - 1] : (seq_t)0;
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        /* pattern index: (k-1) mod 5 */
        last = last * (seq_t)6 + pattern[(k - 1) % 5];
        cache[k - 1] = last;
    }

    /* Update cached_n and return the requested term. */
    cached_n = n;
    return cache[n - 1];
}
