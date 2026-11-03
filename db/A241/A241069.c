/*
 * A241069  Number of partitions p of n into distinct parts such that max(p) =
 *          4*min(p).
 * https://oeis.org/A241069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions p of n into distinct parts such that
 *            max(p) = 4*min(p)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n < 5) return 0;                /* trivial zeros for n < 5 */

    idx_t N = n;
    size_t size = (size_t)(N + 1);

    seq_t *dp = mem_new(seq_t, size);
    seq_t *tmp = mem_new(seq_t, size);
    seq_t *res = mem_new(seq_t, size);
    if (!dp || !tmp || !res) {
        mem_free(dp); mem_free(tmp); mem_free(res);
        return 0;                     /* allocation failed */
    }

    mem_set(dp, 0, size * sizeof(seq_t));
    mem_set(tmp, 0, size * sizeof(seq_t));
    mem_set(res, 0, size * sizeof(seq_t));
    dp[0] = 1;                         /* empty set */

    /* m = 1 : block [2,3] */
    for (idx_t t = 2; t <= 3 && t <= N; ++t) {
        for (idx_t s = N; s >= t; --s) dp[s] += dp[s - t];
    }
    idx_t base = 5;                    /* 5*m */
    if (base <= N) {
        for (idx_t s = 0; s <= N - base; ++s) res[base + s] += dp[s];
    }

    /* m = 2 .. M */
    idx_t M = N / 5;
    for (idx_t m = 2; m <= M; ++m) {
        /* divide dp by (1 + q^m) : remove element m */
        tmp[0] = 1;
        for (idx_t s = 1; s <= N; ++s) {
            if (s < m) {
                tmp[s] = dp[s];
            } else {
                tmp[s] = dp[s] - tmp[s - m];
            }
        }
        /* swap pointers */
        seq_t *swap = dp; dp = tmp; tmp = swap;

        /* add new elements 4*m-4 .. 4*m-1 */
        idx_t start = 4 * m - 4;
        idx_t end   = 4 * m - 1;
        if (start <= N) {
            if (end > N) end = N;
            for (idx_t t = start; t <= end; ++t) {
                for (idx_t s = N; s >= t; --s) dp[s] += dp[s - t];
            }
        }

        /* accumulate contributions for this m */
        base = 5 * m;
        if (base <= N) {
            for (idx_t s = 0; s <= N - base; ++s) res[base + s] += dp[s];
        }
    }

    seq_t ans = res[N];
    mem_free(dp);
    mem_free(tmp);
    mem_free(res);
    return ans;
}
