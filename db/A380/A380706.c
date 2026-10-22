/*
 * A380706  n is the a(n)-th nonnegative integer having its set of decimal
 *          digits.
 * https://oeis.org/A380706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = number of integers m ≤ n whose set of decimal digits
 *            equals the set of digits of n
 * Range:     u8 n <= 9543, u16 n <= 9543, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n = 0 is the only number with digit set {0}. Its rank is 1. */
    if (n == 0) return 1;

    /* Extract digits of n (most significant first). */
    uint8_t dgt[20];
    idx_t tmp = n;
    int len = 0;
    while (tmp > 0) {
        dgt[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = dgt[i];
        dgt[i] = dgt[len - 1 - i];
        dgt[len - 1 - i] = t;
    }

    /* Determine the target digit‑mask. */
    unsigned target_mask = 0;
    for (int i = 0; i < len; ++i)
        target_mask |= 1u << dgt[i];

    /* DP table: dp[pos][tight][started][mask] */
    static seq_t dp[21][2][2][1 << 10];
    memset(dp, 0, sizeof(dp));
    dp[0][1][0][0] = 1;          /* start with no digits taken */

    for (int pos = 0; pos < len; ++pos) {
        for (int tight = 0; tight <= 1; ++tight) {
            int maxdig = tight ? dgt[pos] : 9;
            for (int started = 0; started <= 1; ++started) {
                for (int mask = 0; mask < (1 << 10); ++mask) {
                    seq_t val = dp[pos][tight][started][mask];
                    if (!val) continue;
                    for (int d = 0; d <= maxdig; ++d) {
                        int ntight   = (tight   && d == maxdig) ? 1 : 0;
                        int nstarted = (started || d != 0) ? 1 : 0;
                        int nmask    = mask;
                        if (nstarted) nmask |= 1 << d;
                        dp[pos + 1][ntight][nstarted][nmask] += val;
                    }
                }
            }
        }
    }

    /* Sum all numbers with the exact target digit‑mask. */
    seq_t res = 0;
    for (int tight = 0; tight <= 1; ++tight)
        for (int started = 1; started <= 1; ++started)
            res += dp[len][tight][started][target_mask];

    /* Add zero if its digit set matches. */
    if (target_mask == 1)
        ++res;

    return res;
}
