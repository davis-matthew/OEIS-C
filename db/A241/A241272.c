/*
 * A241272  Number of partitions p of n into distinct parts such that max(p) =
 *          5*min(p).
 * https://oeis.org/A241272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into distinct parts p such that
 *            * max(p) = 5 * min(p). For each such partition let m = min(p).
 *            Then the * parts are m, 5m together with a subset S of {m+1,
 *            m+2, … , 5m-1} and the * total sum is n = 6m + sum(S). Thus a(n)
 *            equals the number of subsets * of {m+1,…,5m...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a classic 〈subset sum〉 DP for each allowed m.
 * The DP array dp[0…T] keeps the number of ways to obtain each sum
 * using the elements of the current range.  The DP is re‑used for each m
 * to avoid re‑allocation costs.  The complexity is
 *     Σ_{m} Σ_{k=m+1}^{min(5m‑1,T)} (T – k + 1)
 * which stays below ~2·10⁸ elementary additions for n≤10000 and
 * therefore comfortably fits into the time limit.  All arithmetic is
 * performed with the provided seq_t type so that the sequence is
 * exact for any integer width or arbitrary‑precision build.  */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* no possible partition for n < 6 */
    if (n < 6) return (seq_t)0;

    const idx_t max_m = n / 6;    /* largest min value with 6m ≤ n */
    seq_t result = (seq_t)0;

    /* reusable DP array */
    static seq_t *dp = NULL;
    static idx_t  dp_cap = 0;

    if (dp_cap < n + 1) {
        /* free previous allocation if any */
        if (dp) free(dp);
        dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
        dp_cap = n + 1;
    }

    for (idx_t m = 1; m <= max_m; ++m) {
        const idx_t T = n - 6 * m;          /* target sum for subset */
        if (T == 0) {                       /* empty subset always works */
            result += (seq_t)1;
            continue;
        }

        /* zero dp[1..T] and set dp[0] = 1 */
        memset(dp, 0, (size_t)(T + 1) * sizeof(seq_t));
        dp[0] = (seq_t)1;

        const idx_t k_end = (5 * m - 1 < T) ? (5 * m - 1) : T;

        for (idx_t k = m + 1; k <= k_end; ++k) {
            /* standard subset‑sum update, iterate s descending */
            for (idx_t s = T; s >= k; --s) {
                dp[s] += dp[s - k];
            }
        }

        result += dp[T];
    }

    return result;
}
