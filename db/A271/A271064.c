/*
 * A271064  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 261", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) - a(n-5) for n > 4
 *            Initial values: a(0)=7, a(1)=-7, a(2)=47, a(3)=-47, a(4)=119
 *            (This recurrence gives the sequence A271064
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

enum { MAX_CACHE = 20001 };   /* enough for all test cases up to n≈10 000 */

static seq_t cache[MAX_CACHE];
static idx_t cache_len = 0;   /* number of terms already computed (indices 0..cache_len-1 are valid) */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialise the first five terms exactly once.  */
    if (cache_len == 0) {
        cache[0] = 7;
        cache[1] = -7;
        cache[2] = 47;
        cache[3] = -47;
        cache[4] = 119;
        cache_len = 5;  /* indices 0 through 4 are now valid */
    }

    /* If the requested term is already cached, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Compute successive terms up to n using the recurrence.  */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* The indices i-1 … i-5 are guaranteed to be <= cache_len-1
           and thus already computed.  */
        cache[i] = -cache[i-1]
                   + 2 * cache[i-2]
                   + 2 * cache[i-3]
                   - cache[i-4]
                   - cache[i-5];
    }
    cache_len = n + 1;   /* all terms up to n are now in cache */
    return cache[n];
}
