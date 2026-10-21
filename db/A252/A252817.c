/*
 * A252817  Number of n X 5 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its city block distance from the upper
 *          left and every value increasing by 0 or 1 with every step right or
 *          down.
 * https://oeis.org/A252817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A252817: Number of n X 5 nonnegative integer arrays with upper left 0
 *          and every value within 2 of its city block distance from the
 *          upper left and every value increasing by 0 or 1 with every
 *          step right or down.
 *
 *  Recurrence (valid for n > 11):
 *      a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4)
 *           + 462*a(n-5) - 462*a(n-6) + 330*a(n-7) - 165*a(n-8)
 *           + 55*a(n-9)  - 11*a(n-10) + a(n-11)
 *
 *  Initial values (offset 1):
 *      a(1) = 11  a(2) = 81  a(3) = 468  a(4) = 2078  a(5) = 7564
 *      a(6) = 23664 a(7) = 65711 a(8) = 165685 a(9) = 385736
 *     a(10)= 839799 a(11)= 1726761
 *
 *  The function memoises computed terms in a static array so that
 *  calls in arbitrary order (including repeated queries) are handled
 *  efficiently.  The implementation uses only the arithmetic operators
 *  allowed for seq_t and casts constant integers to seq_t explicitly.
 */

#include <stdlib.h>

static const seq_t init_vals[11] = {
    (seq_t)11,  (seq_t)81,   (seq_t)468,   (seq_t)2078,   (seq_t)7564,
    (seq_t)23664,(seq_t)65711,(seq_t)165685,(seq_t)385736,(seq_t)839799,
    (seq_t)1726761
};

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* out of range – value irrelevant */

    size_t req = (size_t)n;              /* desired cache size */

    if (cache_len < req) {
        seq_t *new_cache = (seq_t*)realloc(cache, req * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure ignored */
        cache = new_cache;

        for (size_t i = cache_len; i < req; ++i) {
            if (i < 11) {
                cache[i] = init_vals[i];
            } else {
                seq_t sum = (seq_t)0;
                sum = sum + (seq_t)11  * cache[i-1];
                sum = sum - (seq_t)55  * cache[i-2];
                sum = sum + (seq_t)165 * cache[i-3];
                sum = sum - (seq_t)330 * cache[i-4];
                sum = sum + (seq_t)462 * cache[i-5];
                sum = sum - (seq_t)462 * cache[i-6];
                sum = sum + (seq_t)330 * cache[i-7];
                sum = sum - (seq_t)165 * cache[i-8];
                sum = sum + (seq_t)55  * cache[i-9];
                sum = sum - (seq_t)11  * cache[i-10];
                sum = sum + (seq_t)1   * cache[i-11];
                cache[i] = sum;
            }
        }
        cache_len = req;
    }

    return cache[n-1];
}
