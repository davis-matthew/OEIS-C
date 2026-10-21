/*
 * A255117  Number of n-length words on {0,1,2,3,4} in which 0 appears only in
 *          runs of length 2.
 * https://oeis.org/A255117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 4*a(n+2) + 4*a(n), with a(0)=1, a(1)=4, a(2)=17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute OEIS sequence A255117: Number of n‑length words on {0,1,2,3,4}
   in which 0 appears only in runs of length 2. */
seq_t a(idx_t n)
{
    /* Lazy memoisation table */
    static seq_t *cache = NULL;     /* array of values, 0‑indexed */
    static idx_t cache_len   = 0;   /* allocated size (number of slots)   */
    static idx_t computed_to = 0;   /* highest index already computed   */

    /* Ensure table initialised with the first three terms */
    if (cache == NULL) {
        /* Allocate at least three entries for the base cases */
        cache_len = (n + 1 > 3) ? (n + 1) : 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed – return 0 as a safe default */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)17;
        computed_to = 2;
    }

    /* Grow the table if the caller asked for an index beyond the current size */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed – safe default */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing values up to n */
    if (n > computed_to) {
        for (idx_t k = computed_to + 1; k <= n; ++k) {
            /* k >= 3 because the first three terms are already known */
            cache[k] = ((seq_t)4) * cache[k - 1] + ((seq_t)4) * cache[k - 3];
        }
        computed_to = n;
    }

    return cache[n];
}
