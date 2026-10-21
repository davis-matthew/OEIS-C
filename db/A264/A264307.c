/*
 * A264307  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,0 0,1 1,0 -1,-2 or
 *          0,2.
 * https://oeis.org/A264307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 4*a(n-3), * with a(1)=6, a(2)=20,
 *            a(3)=68
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence satisfies a linear recurrence.
 *  Caching is used so calls in any order are O(1) after the
 *  needed terms have been computed once.  The cache grows
 *  geometrically to keep amortised O(1) allocation cost. */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* cached values, 1‑based index */
static idx_t  a_cache_size = 0;   /* allocated capacity (>= index+1) */
static idx_t  a_cache_filled = 0; /* highest index whose value is known */

/* Ensure that the cache has room for indices up to n (inclusive). */
static void a_cache_ensure(idx_t n) {
    if (n + 1 <= a_cache_size)
        return;

    idx_t new_size = a_cache_size ? a_cache_size : 4;
    while (new_size <= n + 1)
        new_size <<= 1;

    a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_size);
    a_cache_size = new_size;
}

/* Fill the cache up to index n using the recurrence. */
static void a_cache_init(idx_t n) {
    if (a_cache_filled >= n)
        return;

    for (idx_t i = a_cache_filled + 1; i <= n; ++i) {
        if (i == 1)
            a_cache[1] = (seq_t)6;
        else if (i == 2)
            a_cache[2] = (seq_t)20;
        else if (i == 3)
            a_cache[3] = (seq_t)68;
        else {
            seq_t v = (seq_t)2 * a_cache[i-1] + (seq_t)4 * a_cache[i-2] + (seq_t)4 * a_cache[i-3];
            a_cache[i] = v;
        }
    }
    a_cache_filled = n;
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;            /* defined only for n >= 1 */

    a_cache_ensure(n);
    a_cache_init(n);
    return a_cache[n];
}
