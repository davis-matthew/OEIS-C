/*
 * A289992  Number of magic labelings of the prism graph I X C_8 having magic
 *          sum n.
 * https://oeis.org/A289992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) (n > 9) Initial values: a(0)=1, a(1)=49, a(2)=746,
 *            a(3)=6122, a(4)=34067, a(5)=144963, a(6)=506772, a(7)=1524628,
 *            a(8)=4074949, a(9)=9898229
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* initial 10 terms */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)49,
        (seq_t)746,
        (seq_t)6122,
        (seq_t)34067,
        (seq_t)144963,
        (seq_t)506772,
        (seq_t)1524628,
        (seq_t)4074949,
        (seq_t)9898229
    };

    /* Allocate or extend cache up to n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failed, return 0 */

        /* Copy existing data */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* If cache was uninitialized, fill initial terms */
        if (cache_size == 0) {
            for (idx_t i = 0; i < 10 && i < new_size; ++i)
                new_cache[i] = init[i];
        }

        /* Compute new terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 10) {
                new_cache[i] = init[i];
            } else {
                new_cache[i] = (seq_t)(10 * new_cache[i-1]
                                - 45 * new_cache[i-2]
                                + 120 * new_cache[i-3]
                                - 210 * new_cache[i-4]
                                + 252 * new_cache[i-5]
                                - 210 * new_cache[i-6]
                                + 120 * new_cache[i-7]
                                - 45  * new_cache[i-8]
                                + 10  * new_cache[i-9]
                                - new_cache[i-10]);
            }
        }

        /* Replace old cache */
        if (cache)
            free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
