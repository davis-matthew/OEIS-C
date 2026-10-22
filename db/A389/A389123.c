/*
 * A389123  Lower (1/3,1/2)-midsequence of ((2^n)) and ((3^n)); see Comments.
 * https://oeis.org/A389123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A389123
    a(n) = floor((1/3)*2^n + (1/2)*3^n)
    Recurrence: a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 6*a(n-4)
    Initial values: a(0)=0, a(1)=2, a(2)=5, a(3)=16
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0..3 */
    static const seq_t init[4] = {0, 2, 5, 16};

    /* For small n return the pre‑computed constant */
    if (n < 4)
        return init[(size_t)n];

    /* dynamic memoisation of all computed terms */
    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0; /* allocated capacity */
    static size_t  cache_len = 0; /* first index beyond cached data */

    size_t nn = (size_t)n;          /* cast idx_t to size_t safely */

    /* grow storage if necessary */
    if (cache_cap <= nn) {
        seq_t *tmp = (seq_t *)realloc(cache, (nn + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure, safe fallback */
        cache = tmp;
        cache_cap = nn + 1;
    }

    /* initialise first four terms on first use */
    if (cache_len == 0) {
        if (cache_cap < 4) {
            seq_t *tmp = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = 4;
        }
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache_len = 4;
    }

    /* compute the missing terms up to n */
    for (size_t i = cache_len; i <= nn; ++i) {
        cache[i] = 5 * cache[i-1] - 5 * cache[i-2] -
                   5 * cache[i-3] + 6 * cache[i-4];
    }
    cache_len = nn + 1;

    return cache[nn];
}
