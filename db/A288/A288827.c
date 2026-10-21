/*
 * A288827  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 515", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4)  for n > 5
 * Initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=13, a(4)=27, a(5)=59
 * This function memoises previously computed terms so any call order is
 * supported.  It uses two static variables: `len` holds the highest
 * index that has been computed (plus one), and `cap` holds the size of the
 * allocated cache.  The cache is doubled in size whenever a larger
 * index is requested.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *cache = NULL;   /* cached values */
    static idx_t len = 0;         /* number of values stored in cache */
    static size_t cap = 0;        /* allocated capacity of cache */

    /* Initialise cache on first call */
    if (len == 0) {
        /* start with space for at least 8 entries */
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* allocation failed */
        /* seed with the first six known terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)27;
        cache[5] = (seq_t)59;
        len = 6;              /* we have terms 0..5 computed */
    }

    /* If the requested term is already stored, return it */
    if (n < len)
        return cache[n];

    /* Ensure capacity large enough for n+1 terms */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        /* double until we have at least n+1 entries */
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to and including index n */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence valid for i > 5; our loop starts at len>=6 */
        cache[i] = 3 * cache[i-1] - 6 * cache[i-3] + 4 * cache[i-4];
    }

    /* Update length of stored values */
    len = n + 1;

    return cache[n];
}
