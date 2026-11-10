/*
 * A365746  Table read by antidiagonals upward: T(n,k) is the number of binary
 *          strings of length k with the property that every substring of
 *          length A070939(n) is lexicographically earlier than the binary
 *          expansion of n; n, k >= 0.
 * https://oeis.org/A365746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   T(n,k) = number of binary strings of length k whose every
 *            length-L window (L = A070939(n)) is < binary(n); computed by DP
 *            over the last L-1 bits. Table read by antidiagonals upward: a(m)
 *            = T(d-j, j) with d(d+1)/2 <= m, j = m - d(d+1)/2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 83 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t T_count(idx_t n, idx_t k)
{
    unsigned L = 0;
    idx_t t = n;
    if (n == 0) L = 1;
    else while (t) { ++L; t >>= 1; }

    if (k + 1 < (idx_t)L) {
        seq_t r = 1;
        for (idx_t i = 0; i < k; i++) r *= 2;
        return r;
    }
    size_t states = (size_t)1 << (L - 1);
    seq_t *dp = mem_new(seq_t, states);
    seq_t *nd = mem_new(seq_t, states);
    for (size_t s = 0; s < states; s++) dp[s] = 1;
    idx_t steps = k - (L - 1);
    for (idx_t st = 0; st < steps; st++) {
        for (size_t s = 0; s < states; s++) nd[s] = 0;
        for (size_t s = 0; s < states; s++) {
            if (dp[s] == 0) continue;
            for (unsigned b = 0; b < 2; b++) {
                idx_t w = ((idx_t)s << 1) | b;
                if (w < n) {
                    size_t ns = (size_t)(w & (idx_t)(states - 1));
                    nd[ns] += dp[s];
                }
            }
        }
        seq_t *tmp = dp; dp = nd; nd = tmp;
    }
    seq_t total = 0;
    for (size_t s = 0; s < states; s++) total += dp[s];
    mem_free(dp);
    mem_free(nd);
    return total;
}

seq_t a(idx_t m)
{
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= m) d++;
    idx_t j = m - d * (d + 1) / 2;
    return T_count(d - j, j);
}
