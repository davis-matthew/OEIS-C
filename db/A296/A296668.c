/*
 * A296668  Number of n X 2 0..1 arrays with each 1 adjacent to 0, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-3) - 10*a(n-4) - 12*a(n-5) - 4*a(n-6)
 *            for n ≥ 7 Initial values: a(1)=3, a(2)=10, a(3)=25, a(4)=68,
 *            a(5)=208, a(6)=609
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len < n) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        cache = tmp;

        /* Initialise the known first six terms if they are not yet present */
        if (cache_len < 6) {
            static const seq_t init[6] = {
                (seq_t)3,   /* a(1) */
                (seq_t)10,  /* a(2) */
                (seq_t)25,  /* a(3) */
                (seq_t)68,  /* a(4) */
                (seq_t)208, /* a(5) */
                (seq_t)609  /* a(6) */
            };
            idx_t i;
            for (i = cache_len; i < (new_len < 6 ? new_len : 6); ++i)
                cache[i] = init[i];
        }

        /* Compute further terms using the recurrence */
        for (idx_t i = (cache_len > 6 ? cache_len : 6); i < new_len; ++i) {
            seq_t term = 3 * cache[i - 1] + 5 * cache[i - 3];
            term -= (10 * cache[i - 4] + 12 * cache[i - 5] + 4 * cache[i - 6]);
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
