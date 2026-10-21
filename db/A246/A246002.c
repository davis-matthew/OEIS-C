/*
 * A246002  Number of length 7+2 0..n arrays with no pair in any consecutive
 *          three terms totalling exactly n.
 * https://oeis.org/A246002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) -20*a(n-3) +56*a(n-5) -28*a(n-6)
 *            -84*a(n-7) +70*a(n-8) +70*a(n-9) -84*a(n-10) -28*a(n-11)
 *            +56*a(n-12) -20*a(n-14) +4*a(n-15) +3*a(n-16) -a(n-17) Initial
 *            values: a(1)=2, a(2)=82, a(3)=5572, a(4)=75260, a(5)=728358,
 *            a(6)=4108062, a(7)=18838280, a(8)=66983128, a(9)=2097321...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 138, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const uint64_t init_vals[17] = {
    2ULL, 82ULL, 5572ULL, 75260ULL, 728358ULL,
    4108062ULL, 18838280ULL, 66983128ULL, 209732170ULL,
    567864650ULL, 1413132492ULL, 3203059092ULL, 6839534702ULL,
    13687432150ULL, 26203871248ULL, 47834595632ULL, 84386904210ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined region */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache large enough for n */
    if (cache_len < n) {
        idx_t new_len = n > 17 ? n : 17;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        cache = tmp;
        cache_len = new_len;
        /* Initialize base values */
        for (idx_t i = 0; i < 17 && i < cache_len; ++i)
            cache[i] = (seq_t)init_vals[i];
        /* Zero remaining positions not yet initialized */
        for (idx_t i = 17; i < cache_len; ++i)
            cache[i] = 0;
    }

    if (n <= 17)
        return cache[n - 1];

    /* Compute missing terms using recurrence */
    for (idx_t i = 17; i < n; ++i) { /* i = 0-based index for a(i+1) */
        seq_t val = 3 * cache[i - 1] + 4 * cache[i - 2] - 20 * cache[i - 3]
                    + 56 * cache[i - 5] - 28 * cache[i - 6]
                    - 84 * cache[i - 7] + 70 * cache[i - 8]
                    + 70 * cache[i - 9] - 84 * cache[i -10]
                    - 28 * cache[i -11] + 56 * cache[i -12]
                    - 20 * cache[i -14] + 4 * cache[i -15]
                    + 3 * cache[i -16] - cache[i -17];
        cache[i] = val;
    }

    return cache[n - 1];
}
