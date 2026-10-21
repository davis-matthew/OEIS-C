/*
 * A254675  Indices of centered triangular numbers (A005448) which are also
 *          heptagonal numbers (A000566).
 * https://oeis.org/A254675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254675: indices of centered triangular numbers that are also heptagonal
   Recurrence: a(n) = a(n-1) + 62*a(n-2) - 62*a(n-3) - a(n-4) + a(n-5)
   Initial values: a(1)=1, a(2)=13, a(3)=44, a(4)=776, a(5)=2697
   (index 0 is unused)
   This implementation memoises computed terms in a static array that grows as needed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: OEIS offset starts at 1.  A(0) is undefined, but we
       return 0 if it is ever requested. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of already computed terms.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of allocated elements (0 … cache_len-1) */

    /* Expand cache when a larger index is requested. */
    idx_t needed = n + 1;                /* indices 0 … n inclusive */

    if (cache_len < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp && needed > 0) abort();    /* allocation failure */

        cache = tmp;

        /* Initialise the newly allocated entries.  Existing entries are
           already valid because we start from the old cache_len. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;          /* unused */
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)13;
            } else if (i == 3) {
                cache[3] = (seq_t)44;
            } else if (i == 4) {
                cache[4] = (seq_t)776;
            } else if (i == 5) {
                cache[5] = (seq_t)2697;
            } else {
                /* recurrence: a(n) = a(n-1) + 62*a(n-2) – 62*a(n-3) – a(n-4) + a(n-5) */
                seq_t v1 = cache[i-1];
                seq_t v2 = cache[i-2];
                seq_t v3 = cache[i-3];
                seq_t v4 = cache[i-4];
                seq_t v5 = cache[i-5];
                cache[i] = v1 + (seq_t)62 * v2 - (seq_t)62 * v3 - v4 + v5;
            }
        }
        cache_len = needed;
    }

    return cache[n];
}
