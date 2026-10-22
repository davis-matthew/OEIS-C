/*
 * A367777  a(n) = |Stirling1(2*n + 1, n)|.
 * https://oeis.org/A367777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |Stirling1(2*n + 1, n)| (ordinary Stirling number of the
 *            first kind)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached array for elementary symmetric sums e[0..n+1] :
   e[r] holds the unsigned elementary symmetric sum of degree r
   of the numbers processed so far (0..current).  We use the
   identity a(n) = e_{n+1}(0,1,2,…,2n).  The recurrence updating
   e is the same as that for the unsigned Stirling numbers of
   the first kind, but with the element value added multiplicatively. */
static seq_t *e_cache = NULL;
static size_t e_cap   = 0;

seq_t a(idx_t n)
{
    /* needed array size: indices 0 .. n+1 inclusive */
    size_t needed = (size_t)n + 2;

    if (e_cap < needed) {
        seq_t *tmp = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        }
        free(e_cache);
        e_cache = tmp;
        e_cap   = needed;
    }

    /* initialise e[0] = 1, others 0 */
    for (size_t i = 0; i < needed; ++i) {
        e_cache[i] = (seq_t)0;
    }
    e_cache[0] = (seq_t)1;

    /* iterate over the set {0,1,…,2n}.  Adding 0 changes nothing, so we
       start from 1.  For each new element a we update the symmetric sums
       from high degree down to 1:  e[r] += a * e[r-1]. */
    for (idx_t a = 1; a <= 2 * n; ++a) {
        size_t rmax = (a < (idx_t)(n + 1)) ? (size_t)a : (size_t)(n + 1);
        for (size_t r = rmax; r > 0; --r) {
            /* use the fact that e[r] is updated in-place from previous
               values stored in e[r-1] (old) because we traverse r downward. */
            seq_t prod = e_cache[r - 1] * (seq_t)a;
            e_cache[r] = e_cache[r] + prod;
        }
    }

    /* e[n+1] is the absolute value of the required Stirling number */
    return e_cache[n + 1];
}
