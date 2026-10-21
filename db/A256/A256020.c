/*
 * A256020  a(n) = Sum_{i=1..n-1} (i^4 * a(i)), a(1)=1.
 * https://oeis.org/A256020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256020
   Formula: a(n) = ∏_{i=2}^{n-1} (i⁴ + 1)
   Recurrence: a(1)=1, a(2)=1
                a(n) = a(n‑1) * (((n-1)⁴) + 1)   for n > 2
   (All indices are 1‑based.) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised values: cache[i] stores a(i).  Index 0 is unused. */
    static seq_t *cache = NULL;   /* allocated array of seq_t */
    static idx_t cache_len = 0;   /* highest index that has a known value */
    static size_t cache_cap = 0;  /* allocated capacity (highest usable index + 1) */

    if (n <= 2)                /* trivial cases */
        return (seq_t)1;

    /* enlarge allocation if necessary */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                 /* allocation failure – should not happen */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first two entries if they are not yet present */
    if (cache_len < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_len = 2;
    }

    /* compute all missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t ip   = (seq_t)(i - 1);      /* (i-1) as seq_t   */
        seq_t ip2  = ip * ip;             /* (i-1)²          */
        seq_t ip4  = ip2 * ip2;           /* (i-1)⁴          */
        cache[i] = cache[i - 1] * (ip4 + (seq_t)1);
    }
    cache_len = n;

    return cache[n];
}
