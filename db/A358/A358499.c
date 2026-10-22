/*
 * A358499  a(n) = Sum_{k=0..floor(n/4)} (n-4*k)!.
 * https://oeis.org/A358499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A358499: a(n) = Σ_{k=0..⌊n/4⌋} (n‑4k)!.
 * The values satisfy the recurrence
 *   a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=25,
 *   a(n) = n*a(n‑1) + a(n‑4) – n*a(n‑5)   for n>4.
 * The function memoises the values in a static array so that
 * subsequent calls (in any order) are O(1) per term.
 */

#include <stdlib.h>   /* malloc, realloc, free, size_t */

static seq_t *cache = NULL;   /* memoisation table, indexed 0.. */
static size_t cache_cap = 0;  /* current allocated size (number of slots) */

/* Compute a(n) for any n ≥ 0 */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough for index n */
    if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        /* Grow to at least double the current capacity or to n+1 */
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {            /* allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;

        /* Initialise newly allocated slots only when needed.
           The actual values will be computed below. */
        (void)old_cap;  /* silence unused variable warning */
    }

    /* Compute missing terms up to n (in increasing order). */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)2;
        } else if (i == 3) {
            cache[3] = (seq_t)6;
        } else if (i == 4) {
            cache[4] = (seq_t)25;
        } else {
            /* Use the recurrence only for i ≥ 5 */
            cache[i] = ((seq_t)i * cache[i-1])          /* n*a(n-1) */
                     + cache[i-4]                       /* + a(n-4) */
                     - ((seq_t)i * cache[i-5]);         /* - n*a(n-5) */
        }
    }

    return cache[(size_t)n];
}
