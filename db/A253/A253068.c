/*
 * A253068  The subsequence A253066(2^n-1).
 * https://oeis.org/A253068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 1, 6, 28, 3a(n-1)+6a(n-2)-8a(n-3) (n≥3) }
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
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

    /* extend cache if needed */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely in tests */

        /* initialise missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)               cache[i] = 1;
            else if (i == 1)          cache[i] = 6;
            else if (i == 2)          cache[i] = 28;
            else {                    /* recurrence for i ≥ 3 */
                seq_t temp = cache[i-1] * 3;
                temp += cache[i-2] * 6;
                temp -= cache[i-3] * 8;
                cache[i] = temp;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
