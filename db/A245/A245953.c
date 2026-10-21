/*
 * A245953  Number of length 3+3 0..n arrays with some pair in every
 *          consecutive four terms totalling exactly n.
 * https://oeis.org/A245953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 80, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A245953: Empirical recurrence
   a(n) = 3*a(n-1) - 8*a(n-3) + 6*a(n-4) + 6*a(n-5) - 8*a(n-6)
          + 3*a(n-8) - a(n-9);

   Initial values:
   a(1)=48, a(2)=545, a(3)=2304, a(4)=7769, a(5)=18384,
   a(6)=39721, a(7)=73728, a(8)=130193, a(9)=211440.
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == 0) return 0;
    if (n <= 9) {
        static const seq_t base[10] = {
            0,                    /* a(0) unused */
            48, 545, 2304, 7769, 18384,
            39721, 73728, 130193, 211440
        };
        return base[n];
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated length */
    static idx_t computed_max = 0;    /* highest index resolved */

    /* Ensure base values present for indices 1..9 */
    if (cache_size < 10) {
        seq_t *tmp = (seq_t *)realloc(cache, 10 * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_size = 10;
        static const seq_t base[10] = {
            0,                    /* a(0) unused */
            48, 545, 2304, 7769, 18384,
            39721, 73728, 130193, 211440
        };
        for (size_t i = 1; i <= 9; ++i) cache[i] = base[i];
        computed_max = 9;
    }

    /* Grow cache if necessary */
    if (cache_size < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute missing terms */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t v = 3 * cache[i - 1];
            v -= 8 * cache[i - 3];
            v += 6 * cache[i - 4];
            v += 6 * cache[i - 5];
            v -= 8 * cache[i - 6];
            v += 3 * cache[i - 8];
            v -= cache[i - 9];
            cache[i] = v;
        }
        computed_max = n;
    }

    return cache[n];
}
