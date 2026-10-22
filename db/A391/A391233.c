/*
 * A391233  Number of strict integer partitions of n > 0 such that the least
 *          part and the greatest part are both even.
 * https://oeis.org/A391233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of strict integer partitions of n > 0
   with both the smallest and the largest part even.  */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset == 1, so a(1) is first term */

    /* dp[sum][min_parity][max_parity]  parity: 0=even, 1=odd, 2=none yet */
    const size_t stride = 9;             /* 3 min × 3 max */
    const size_t total  = (size_t)(n + 1) * stride;

#if defined(__cplusplus)
    seq_t *dp = new seq_t[total]();      /* value‑initialize all elements */
#else
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));  /* zero initialize */
#endif

    const size_t NONE = 2;
    /* empty subset has no min and no max */
    dp[(size_t)0 * stride + NONE * 3 + NONE] = (seq_t)1;

    for (idx_t part = 1; part <= n; ++part) {
        const size_t part_par = (size_t)(part & 1);            /* 0 even, 1 odd */

        /* iterate sums in descending order to avoid reusing the same part */
        for (idx_t s = n; s >= part; --s) {
            const size_t s_minus_part = (size_t)(s - part);
            const size_t base_old = s_minus_part * stride;
            const size_t base_new = s * stride;

            for (int min_par = 0; min_par < 3; ++min_par) {
                for (int max_par = 0; max_par < 3; ++max_par) {
                    seq_t val = dp[base_old + min_par * 3 + max_par];
                    if (!val) continue;

                    /* new minimum parity: if none yet, it's the current part */
                    const int new_min_par = (min_par == (int)NONE) ? (int)part_par : min_par;
                    /* new maximum parity becomes the current part */
                    const int new_max_par = (int)part_par;

                    dp[base_new + new_min_par * 3 + new_max_par] += val;
                }
            }

            if (s == part) break;   /* avoid unsigned wrap‑around */
        }
    }

    seq_t result = dp[(size_t)n * stride + 0 * 3 + 0];  /* min even, max even */

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
