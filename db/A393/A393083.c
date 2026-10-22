/*
 * A393083  Anarboricity of the n-triangular honeycomb rook graph.
 * https://oeis.org/A393083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1)-3*a(n-2)+2*a(n-3)-3*a(n-4)+3*a(n-5)-a(n-6) for n>=10
   Initial values: a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=5,
   a(6)=10, a(7)=17, a(8)=26, a(9)=38.
   The values for n<4 are also defined by the given table.
   This implementation memoises computed values in a static array,
   allowing arbitrary order of calls with O(n) total work per new n. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    if (n == 0) { /* offset is 1, but return 0 for n==0 just in case */
        return 0;
    }

    /* Grow cache if necessary */
    size_t target = (size_t)n;            /* cache is 1‑indexed, size n means indices 0..n-1 */
    if (target > cached_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort */
            abort();
        }
        cache = new_cache;
        /* Constants for indices 1..9 (array indices 0..8) */
        static const seq_t base[9] = {0, 0, 1, 2, 5, 10, 17, 26, 38};
        /* Initialise newly added entries */
        for (size_t i = cached_len + 1; i <= target; ++i) {
            if (i <= 9) {
                cache[i - 1] = base[i - 1];
            } else {
                /* Recurrence: a(n) = 3*a(n-1)-3*a(n-2)+2*a(n-3)-3*a(n-4)+3*a(n-5)-a(n-6) */
                seq_t t = 3 * cache[i - 2] + 2 * cache[i - 4] + 3 * cache[i - 6];
                seq_t sub = 3 * cache[i - 3] + 3 * cache[i - 5] + cache[i - 7];
                cache[i - 1] = t - sub;
            }
        }
        cached_len = target;
    }

    return cache[n - 1];
}
