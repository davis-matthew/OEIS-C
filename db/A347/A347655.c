/*
 * A347655  Number of partitions of n into at most 4 squarefree parts.
 * https://oeis.org/A347655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 4 squarefree
 *            parts. The implementation uses a standard coin‑change dynamic
 *            programme that counts, for each sum, the number of ways to reach
 *            it with exactly k parts, 0 ≤ k ≤ 4
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is an idx_t (unsigned) but we need size_t for array handling */
    size_t N = (size_t)n;

    /* Special case n == 0 */
    if (N == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* ---------- 1️⃣ Mark squarefree numbers up to n ---------- */
    bool *is_sf = (bool *)calloc(N + 1, sizeof(bool));
    if (!is_sf)
        return (seq_t)0;   /* memory allocation failure */

    /* Initially mark all numbers ≥1 as squarefree */
    for (size_t i = 1; i <= N; ++i)
        is_sf[i] = true;

    /* Sieve out multiples of squares */
    for (size_t p = 2; p * p <= N; ++p) {
        size_t sq = p * p;
        for (size_t m = sq; m <= N; m += sq)
            is_sf[m] = false;
    }

    /* ---------- 2️⃣ DP: dp[sum*5 + parts] ------------------ */
    /* 5 because we store counts for k = 0 … 4 */
    seq_t *dp = (seq_t *)calloc((N + 1) * 5, sizeof(seq_t));
    if (!dp) {
        free(is_sf);
        return (seq_t)0;   /* memory allocation failure */
    }

    /* Base case: one way to reach 0 with 0 parts */
    dp[0] = (seq_t)1;     /* dp[sum=0][k=0] */

    /* Process each squarefree number as an unbounded coin type */
    for (size_t v = 1; v <= N; ++v) {
        if (!is_sf[v])
            continue;               /* skip non‑squarefree numbers */

        for (size_t s = v; s <= N; ++s) {
            size_t base   = s * 5;
            size_t prev   = (s - v) * 5;
            /* Add v into partitions up to 4 parts */
            for (size_t k = 1; k <= 4; ++k) {
                dp[base + k] += dp[prev + (k - 1)];
            }
        }
    }

    /* ---------- 3️⃣ Sum over at most 4 parts ---------------- */
    seq_t ans = 0;
    for (size_t k = 0; k <= 4; ++k)
        ans += dp[N * 5 + k];

    /* ---------- 4️⃣ Clean‑up --------------------------------- */
    free(is_sf);
    free(dp);

    return ans;
}
