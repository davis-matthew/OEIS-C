/*
 * A282911  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 526", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 11
   a(2) = 111
   a(3) = 1101
   a(4) = 11001
   a(5) = 111011
   for n > 5:
   a(n) = a(n-1) + 10000*a(n-4) - 10000*a(n-5)
*/
#include <stdlib.h>

static seq_t *cache   = NULL;   /* allocated array of computed terms */
static size_t  cap    = 0;      /* current allocation size (in terms) */
static size_t  len    = 0;      /* number of terms actually stored */

seq_t a(idx_t n)
{
    /* initialise cache with the six known terms if needed */
    if (cache == NULL) {
        const size_t init_cap = 6;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */

        cache[0] = 1;
        cache[1] = 11;
        cache[2] = 111;
        cache[3] = 1101;
        cache[4] = 11001;
        cache[5] = 111011;

        cap = init_cap;
        len = 6;
    }

    /* return value if already computed */
    if ((size_t)n < len) {
        return cache[(size_t)n];
    }

    /* make sure we have space for the requested index */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        size_t idx = (size_t)i;            /* index into the array */
        cache[idx] = cache[idx-1]
                   + ((seq_t)10000) * cache[idx-4]
                   - ((seq_t)10000) * cache[idx-5];
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
