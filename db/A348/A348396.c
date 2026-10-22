/*
 * A348396  Number of ways to reach n by starting with 1 and repeatedly adding
 *          any positive integer or multiplying by any integer greater than 1.
 * https://oeis.org/A348396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ways to reach n by starting with 1 and
 *            repeatedly * adding any positive integer or multiplying by any
 *            integer greater than 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    size_t sz = (size_t)n + 1;           /* indices 0..n, index 0 unused */

    /* allocate arrays for dp and difference array add */
    seq_t *dp  = (seq_t*)alloc_seq_array(sz);          /* dp[1..n] */
    seq_t *add = (seq_t*)alloc_seq_array(sz);          /* add[1..n]  */

    if (!dp || !add) return 0;           /* allocation failed */

    /* initialise */
    for (size_t i = 0; i < sz; ++i) {
        dp[i]  = 0;
        add[i] = 0;
    }
    dp[1] = 1;                           /* base case */

    seq_t current_add = 0;

    for (idx_t t = 1; t <= n; ++t) {
        size_t ti = (size_t)t;

        /* apply all previous addition contributions */
        current_add += add[ti];
        dp[ti] += current_add;

        /* propagate addition to all later values */
        if (ti + 1 < sz)
            add[ti + 1] += dp[ti];

        /* propagate multiplication contributions */
        if (dp[ti] != 0) {
            for (idx_t mult = 2; t * mult <= n; ++mult) {
                size_t idx = (size_t)(t * mult);
                dp[idx] += dp[ti];
            }
        }
    }

    seq_t result = dp[(size_t)n];

    /* free allocated memory */
    free(dp);
    free(add);

    return result;
}
