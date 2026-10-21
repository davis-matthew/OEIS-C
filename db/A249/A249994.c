/*
 * A249994  Expansion of 1/((1-2*x)*(1+3*x)*(1-4*x)).
 * https://oeis.org/A249994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249994:  Expansion of 1/((1-2*x)*(1+3*x)*(1-4*x))
   a(n) = 3*a(n-1) + 10*a(n-2) - 24*a(n-3)
   a(0) = 1, a(1) = 3, a(2) = 19 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoizing results across calls and for
       out‑of‑order queries.  The cache stores the values for
       indices 0 .. (computed_len-1).  The allocated buffer length
       may be larger than the number of computed entries. */
    static seq_t *cache = NULL;
    static idx_t allocated_len = 0;  /* size of the allocated buffer */
    static idx_t computed_len  = 0;  /* highest index + 1 that has been computed */

    /* Initialise base cases lazily.  Allocate space for the first three
       terms and store them. */
    if (cache == NULL) {
        allocated_len = 3;
        cache = (seq_t*)malloc(allocated_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure → 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)19;
        computed_len = 3;
    }

    /* If the requested index is already computed, return it immediately. */
    if (n < computed_len) {
        return cache[n];
    }

    /* Ensure the buffer is large enough to hold up to index n. */
    if ((idx_t)allocated_len <= n) {
        idx_t new_allocated_len = allocated_len * 2;
        if (new_allocated_len <= n) new_allocated_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_allocated_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure → 0 */
        cache = tmp;
        allocated_len = new_allocated_len;
    }

    /* Compute all needed terms up to n using the recurrence. */
    for (idx_t i = computed_len; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t val = (seq_t)3 * a1 + (seq_t)10 * a2;
        val -= (seq_t)24 * a3;
        cache[i] = val;
    }
    computed_len = n + 1;   /* all indices up to n inclusive are now valid */

    return cache[n];
}
