/*
 * A277236  Number of strings of length n composed of symbols from the
 *          circular list [1,2,3,4] such that adjacent symbols in the string
 *          must be adjacent in the list. No runs of length 2 or more are
 *          allowed for symbols 1 and 3.
 * https://oeis.org/A277236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2), a(0)=1, a(1)=4, a(2)=10
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* memoized values */
    static idx_t  capacity = 0;    /* allocated array size */
    static idx_t  max_idx   = 0;    /* highest index computed */

    /* Initialise cache on first call */
    if (!cache) {
        capacity = 4;                           /* enough for indices 0..3 */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
        max_idx = 2;                            /* last computed index */
    }

    /* Return value directly if already computed */
    if (n <= max_idx) return cache[n];

    /* Grow cache if necessary */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity << 1 : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* reallocation failure */
        cache    = tmp;
        capacity = new_cap;
    }

    /* Compute values up to n using the recurrence */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + (cache[i-2] * (seq_t)4);
    }
    max_idx = n;
    return cache[n];
}
