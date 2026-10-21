/*
 * A258635  Number of length 4 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 4‑tuples (x1,…,x4) with 1≤xi≤n+1 such that * for
 *            every k = 1..4 the partial sum S_k = x1+…+xk is divisible by *
 *            2, 3, 5, or 7. The computation is performed by dynamic
 *            programming * over residues modulo the least common multiple 210
 *            of (2,3,5,7)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The modulus 210 contains all divisibility patterns of 2,3,5,7. */
    const int MOD = 210;

    seq_t m = (seq_t)n + 1;        /* maximal array entry */

    /* Count how many numbers in 1..m have each residue modulo 210. */
    seq_t cnt[MOD];
    bool   allowed[MOD];

    for (int r = 0; r < MOD; ++r) {
        if (r == 0) {
            cnt[0] = m / MOD;
        } else if ((idx_t)r <= m) {
            cnt[r] = ((m - (seq_t)r) / MOD) + 1;
        } else {
            cnt[r] = 0;
        }
        allowed[r] = (r % 2 == 0) || (r % 3 == 0) || (r % 5 == 0) || (r % 7 == 0);
    }

    /* DP arrays for the current and previous position. */
    seq_t dp_prev[MOD];
    seq_t dp_curr[MOD];

    for (int i = 0; i < MOD; ++i) dp_prev[i] = 0;

    /* Position 1: the element itself must satisfy the divisibility condition. */
    for (int r = 0; r < MOD; ++r)
        if (allowed[r])
            dp_prev[r] = cnt[r];

    /* Positions 2 to 4. */
    for (int step = 2; step <= 4; ++step) {
        for (int i = 0; i < MOD; ++i) dp_curr[i] = 0;
        for (int r1 = 0; r1 < MOD; ++r1) if (dp_prev[r1]) {
            seq_t val = dp_prev[r1];
            for (int r2 = 0; r2 < MOD; ++r2) if (cnt[r2]) {
                int new_res = (r1 + r2) % MOD;
                if (allowed[new_res]) {
                    dp_curr[new_res] = dp_curr[new_res] + val * cnt[r2];
                }
            }
        }
        for (int i = 0; i < MOD; ++i) dp_prev[i] = dp_curr[i];
    }

    /* Sum of all ways after the 4th element. */
    seq_t result = 0;
    for (int r = 0; r < MOD; ++r) result += dp_prev[r];
    return result;
}
