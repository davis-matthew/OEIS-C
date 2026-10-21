/*
 * A266749  G.f.:
 *          1/((1-t^9)^2*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)*(1-t^11)*(1-t^13)*(1-t^15)*(1-t^17)).
 * https://oeis.org/A266749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.:
 *            1/((1-t^9)^2*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)*(1-t^11)*(1-t^13)*(1-t^15)*(1-t^17))
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* DP array for coefficients of the generating function.
       dp[i] will hold the number of partitions of i with the
       parts {1,3,5,7,9,11,13,15,17} where part 9 has multiplicity 2.
       The recurrence is obtained by repeatedly multiplying by
       1/(1-t^s), performing the operation twice for part 9. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;                     /* memory failure: return 0 */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* list of parts with their multiplicities (k=1 except for 9) */
    static const struct { idx_t s; unsigned k; } parts[] = {
        {1, 1}, {3, 1}, {5, 1}, {7, 1}, {9, 2},
        {11, 1}, {13, 1}, {15, 1}, {17, 1}
    };
    const size_t part_count = sizeof(parts) / sizeof(parts[0]);

    for (size_t p = 0; p < part_count; ++p) {
        idx_t step = parts[p].s;
        for (unsigned copy = 0; copy < parts[p].k; ++copy) {
            for (idx_t i = step; i <= n; ++i) {
                dp[i] += dp[i - step];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
