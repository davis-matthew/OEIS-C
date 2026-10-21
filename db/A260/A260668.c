/*
 * A260668  Number of binary words of length n such that for every prefix the
 *          number of occurrences of subword 101 is larger than or equal to
 *          the number of occurrences of subword 010.
 * https://oeis.org/A260668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A260668
   a(n) = number of binary words of length n such that for every prefix
          the number of occurrences of subword 101 is greater than or
          equal to the number of occurrences of subword 010.
   Computation is by dynamic programming over the difference
   (#101 − #010) and the last two bits. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

seq_t a(idx_t n)
{
    /* Small base cases */
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)2;
    if (n == 2ULL) return (seq_t)4;

    /* Maximum difference that can occur for length n */
    size_t maxDiff = (size_t)(n / 3) + 5;

#ifdef __cplusplus
    /* DP tables: dp[code][d] – number of ways to reach state 'code'
       (last two bits) with difference 'd' after some prefix. */
    std::vector<std::vector<seq_t>> dp(4, std::vector<seq_t>(maxDiff + 1));
    std::vector<std::vector<seq_t>> ndp(4, std::vector<seq_t>(maxDiff + 1));

    /* Initial states: any pair of first two bits, difference 0. */
    for (int code = 0; code < 4; ++code)
        dp[code][0] = (seq_t)1;
#else
    /* C implementation uses raw arrays.  Those can be used only
       when seq_t is a POD type (e.g. uint64_t). */
    seq_t *dp[4];
    seq_t *ndp[4];
    for (int i = 0; i < 4; ++i) {
        dp[i]  = (seq_t*)calloc(maxDiff + 1, sizeof(seq_t));
        ndp[i] = (seq_t*)calloc(maxDiff + 1, sizeof(seq_t));
    }

    for (int code = 0; code < 4; ++code)
        dp[code][0] = (seq_t)1;
#endif

    /* Append bits from position 2 up to n-1 */
    for (idx_t pos = 2; pos < n; ++pos) {
#ifdef __cplusplus
        for (int i = 0; i < 4; ++i)
            std::fill(ndp[i].begin(), ndp[i].end(), (seq_t)0);
#else
        for (int i = 0; i < 4; ++i)
            memset(ndp[i], 0, (maxDiff + 1) * sizeof(seq_t));
#endif

        /* Iterate over current states */
        for (int code = 0; code < 4; ++code) {
            int b1 = (code >> 1) & 1;  /* first of the two last bits */
            int b2 =  code        & 1; /* second of the two last bits */

            for (size_t d = 0; d <= maxDiff; ++d) {
                seq_t cur;
#ifdef __cplusplus
                cur = dp[code][d];
#else
                cur = dp[code][d];
#endif
                if (cur == (seq_t)0) continue;

                /* Try appending 0 or 1 */
                for (int bit = 0; bit < 2; ++bit) {
                    int inc = (b1 == 1 && b2 == 0 && bit == 1) ? 1 : 0;
                    int dec = (b1 == 0 && b2 == 1 && bit == 0) ? 1 : 0;
                    int nd  = (int)d + inc - dec;
                    if (nd < 0 || nd > (int)maxDiff) continue;

                    int newcode = (b2 << 1) | bit;
#ifdef __cplusplus
                    ndp[newcode][nd] += cur;
#else
                    ndp[newcode][nd] += cur;
#endif
                }
            }
        }

#ifdef __cplusplus
        /* Prepare for next iteration */
        for (int i = 0; i < 4; ++i)
            dp[i].swap(ndp[i]);
#else
        for (int i = 0; i < 4; ++i)
            memcpy(dp[i], ndp[i], (maxDiff + 1) * sizeof(seq_t));
#endif
    }

    /* Sum over all final states */
    seq_t total = (seq_t)0;
    for (int code = 0; code < 4; ++code) {
        for (size_t d = 0; d <= maxDiff; ++d) {
#ifdef __cplusplus
            total += dp[code][d];
#else
            total += dp[code][d];
#endif
        }
    }

#ifdef __cplusplus
    /* vectors free themselves */
#else
    for (int i = 0; i < 4; ++i) {
        free(dp[i]);
        free(ndp[i]);
    }
#endif

    return total;
}
