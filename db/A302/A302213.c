/*
 * A302213  Number of 3Xn 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4a(n-1)+4a(n-2)-29a(n-3)-3a(n-4)+96a(n-5)-14a(n-6)-187a(n-7)+39a(n-8)+246a(n-9)-52a(n-10)-230a(n-11)+48a(n-12)+146a(n-13)-29a(n-14)-57a(n-15)+9a(n-16)+12a(n-17)-a(n-18)-a(n-19)
 *            for n>20; initial values: a(1)=4, a(2)=2, a(3)=16, a(4)=5,
 *            a(5)=32, a(6)=22, a(7)=72, a(8)=101, a(9)=216, a(10)=4...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 50, u128 n <= 97,
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

    /* initial values for 1 <= n <= 20 */
    static const seq_t init[21] = {
        0,                  /* dummy for index 0 */
        4, 2, 16, 5, 32, 22, 72, 101, 216, 486, 968,
        2541, 5598, 14718, 35506, 91947, 231984, 597758,
        1533828, 3954067
    };

    /* coefficients of the empirical linear recurrence of order 19 */
    static const long coeff[19] = {
        4, 4, -29, -3, 96, -14, -187, 39,
        246, -52, -230, 48, 146, -29, -57, 9, 12, -1, -1
    };

    /* static cache holding the values that have been computed already */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* number of elements allocated (size) */

    idx_t needed = n + 1;              /* we need indices 0..n inclusive */

    if (cache_len < needed) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)needed * sizeof(seq_t));
        if (!new_cache)               /* allocation failure – return 0 */
            return (seq_t)0;
        cache = new_cache;

        /* fill in the known initial values */
        idx_t limit = 20;
        if (limit > n)
            limit = n;
        for (idx_t i = 1; i <= limit; ++i)
            cache[i] = init[i];

        /* compute any remaining terms using the recurrence */
        for (idx_t i = cache_len; i < needed; ++i) {
            if (i <= 20)                /* already set */
                continue;
            seq_t val = 0;
            for (int k = 0; k < 19; ++k)
                val += (seq_t)coeff[k] * cache[i - 1 - k];
            cache[i] = val;
        }
        cache_len = needed;
    }

    return cache[n];
}
