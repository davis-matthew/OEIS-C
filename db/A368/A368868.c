/*
 * A368868  Number of partitions of n into odd parts not greater than sqrt(n).
 * https://oeis.org/A368868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd parts not greater than
 *            sqrt(n). The routine works for any unsigned 64‑bit index n. It
 *            uses dynamic allocation when compiling as C and a std::vector
 *            when compiling as C++
 * Range:     u8 n <= 48, u16 n <= 134, u32 n <= 416, u64 n <= 1368,
 *            u128 n <= 4760, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#ifdef __cplusplus
#  include <vector>
#endif

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)1;

    /* array of length n+1 */
    size_t len = (size_t)n + 1;
#ifdef __cplusplus
    std::vector<seq_t> dp(len, (seq_t)0);
    dp[0] = (seq_t)1;
#else
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure: return 0 */
    for (size_t i = 0; i < len; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;
#endif

    /* odd parts up to floor(sqrt(n)) */
    idx_t max_part = (idx_t)floor(sqrt((double)n));  /* sqrt uses double, safe for n ≤ 10⁴ */

    for (idx_t part = 1; part <= max_part; part += 2) {
        for (idx_t i = part; i <= n; ++i) {
#ifdef __cplusplus
            dp[(size_t)i] += dp[(size_t)(i - part)];
#else
            dp[(size_t)i] = dp[(size_t)i] + dp[(size_t)(i - part)];
#endif
        }
    }

    seq_t result;
#ifdef __cplusplus
    result = dp[(size_t)n];
#else
    result = dp[(size_t)n];
    free(dp);
#endif
    return result;
}
