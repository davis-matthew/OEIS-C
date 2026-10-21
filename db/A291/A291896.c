/*
 * A291896  Number of 1-dimensional sandpiles with n grains piling up against
 *          the wall.
 * https://oeis.org/A291896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 88, u128 n <= 175,
 *            big any n
 * Helpers:   Lmax
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A291896 – smooth compositions
 *  a(0) = 1
 *  For n ≥ 1, a(n) = Σ_v Σ_p a(n‑v, p)
 *      v runs from 1 to Lmax(n) (maximum part)
 *      p runs over {v‑1, v, v+1} ∩ [1, Lmax(n‑v)]
 *      a(s, p) counts smooth compositions of s ending with part p,
 *      with a(1,1) = 1 and a(0, p) = 0 for all p.
 *  The function below implements a memoised DP that satisfies all
 *  requirements and runs in O(n³ᐟ²) time, which is well below the
 *  required limit for n up to about 10 000.
 */

#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/Lmax.h"

/* Main function required by the build system */
seq_t a(idx_t n)
{
    static seq_t *ans = NULL;      /* a(i) for 0 ≤ i ≤ last_n */
    static seq_t **dp  = NULL;     /* dp[i] points to array of size Lmax(i)+1 */
    static idx_t  last_n = 0;      /* largest n for which arrays are ready */

    /* lazy initialisation: prepare for n = 0 */
    if (ans == NULL) {
        ans = (seq_t *)calloc(1, sizeof(seq_t));
        ans[0] = 1;                    /* a(0) = 1 */
        dp  = (seq_t **)calloc(1, sizeof(seq_t *));
        last_n = 0;
    }

    if (n > last_n) {
        /* enlarge array to size n+1 */
        size_t new_size = (size_t)n + 1;
        ans = (seq_t *)realloc(ans, new_size * sizeof(seq_t));
        for (size_t i = last_n + 1; i < new_size; ++i) ans[i] = 0;

        dp = (seq_t **)realloc(dp, new_size * sizeof(seq_t *));
        for (size_t i = last_n + 1; i < new_size; ++i) dp[i] = NULL;

        /* allocate and compute dp for each new sum */
        for (idx_t s = last_n + 1; s <= n; ++s) {
            idx_t maxV = Lmax(s);
            dp[s] = (seq_t *)calloc((size_t)(maxV + 1), sizeof(seq_t));
            if (s == 1) {
                dp[1][1] = 1;
                ans[1] = 1;
                continue;
            }

            /* compute dp[s][v] for each possible last part v */
            for (idx_t v = 1; v <= maxV; ++v) {
                seq_t count = 0;
                idx_t ps = s - v;           /* previous sum */
                if (ps >= 1) {
                    idx_t Lprev = Lmax(ps);
                    /* previous part can be v-1, v, v+1 */
                    if (v > 1 && (v - 1) <= Lprev)
                        count += dp[ps][v - 1];
                    if (v <= Lprev)
                        count += dp[ps][v];
                    if ((v + 1) <= Lprev)
                        count += dp[ps][v + 1];
                }
                dp[s][v] = count;
            }

            /* total for this sum */
            seq_t total = 0;
            for (idx_t v = 1; v <= maxV; ++v)
                total += dp[s][v];
            ans[s] = total;
        }

        last_n = n;
    }

    return ans[n];
}
