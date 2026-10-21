/*
 * A292764  Minimal number of moves for the cyclic variant of Hanoi's tower
 *          for 4 pegs and n disks, with the final peg two steps away.
 * https://oeis.org/A292764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A292764 - minimal number of moves for the cyclic variant
   of Hanoi's tower for 4 pegs, final peg two steps away.
   Recurrence (valid for n > 10):
   a(n) = a(n-1) + a(n-2) + a(n-3) - 2*a(n-5)
   Initial values:
   a(1)=2, a(2)=8, a(3)=18, a(4)=36, a(5)=66,
   a(6)=120, a(7)=210, a(8)=360, a(9)=618, a(10)=1052
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    /* cache of already computed terms (indices start at 1) */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    /* lazy allocation / growth of the cache */
    if (n > cache_len) {
        idx_t new_len = n + 1;                  /* allocate up to index n */
        if (!cache) {
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache,
                                     new_len * sizeof(seq_t));
        }
        /* ensure allocation succeeded */
        if (!cache) return (seq_t)0;            /* allocation failure */

        /* initialize new entries */
        static const seq_t init[10] = {
            (seq_t)2,   (seq_t)8,   (seq_t)18,  (seq_t)36,
            (seq_t)66,  (seq_t)120, (seq_t)210, (seq_t)360,
            (seq_t)618, (seq_t)1052
        };

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = init[i - 1];
            } else {
                /* recurrence: a(i) = a(i-1) + a(i-2) + a(i-3) - 2*a(i-5) */
                seq_t val = cache[i - 1] + cache[i - 2] + cache[i - 3];
                val -= (seq_t)2 * cache[i - 5];
                cache[i] = val;
            }
        }
        cache_len = n;
    }

    return cache[n];
}
