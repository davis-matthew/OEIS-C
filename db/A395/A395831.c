/*
 * A395831  Number of octal strings of length n that do not contain a specific
 *          string xx (where x is a single digit).
 * https://oeis.org/A395831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(n)=7*(a(n-1)+a(n-2)) for n≥2 (Number of octal
 *            strings of length n that do not contain a fixed double digit xx,
 *            where x is a single digit.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The return type is seq_t, and the index type is idx_t.  Do not modify.
   The following static variables are used for memoising the sequence. */
static seq_t *cache = NULL;      /* dynamic array to store computed terms   */
static size_t cache_cap = 0;     /* number of allocated slots in cache     */
static idx_t  computed = 0;      /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* Allocate or grow the cache if necessary.  The array must have
       at least n+1 slots (indices 0..n). */
    if (cache_cap <= (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out‑of‑memory; unlikely here   */
        cache     = tmp;
        cache_cap = (size_t)n + 1;
    }

    /* Initialise the first two terms if they have not been set yet. */
    if (computed == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        computed = 0;
    }
    if (computed == 0 && n >= 1) {
        cache[1] = (seq_t)8;   /* a(1) */
        computed = 1;
    }

    /* Compute any missing terms up to position n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = ((cache[i-1] + cache[i-2]) * (seq_t)7);
    }
    computed = n;

    return cache[n];
}
