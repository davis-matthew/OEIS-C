/*
 * A247922  Number of length n+4 0..3 arrays with some disjoint pairs in every
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   uses DP over 4^4 states to count arrays of length n+4 in which
 *            every consecutive five terms contain two disjoint pairs having
 *            equal sums
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 31, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Length of the array to be counted. */
    idx_t L = n + 4;   /* n >= 1 guarantees L >= 5 */

    /* Lazy initialization of helper tables. */
    static bool init = false;
    static bool good[1024];
    static int16_t trans[256][4];

    if (!init) {
        /* Precompute whether each 5‑tuple satisfies the pair condition. */
        for (int a = 0; a < 4; a++)
            for (int b = 0; b < 4; b++)
                for (int c = 0; c < 4; c++)
                    for (int d = 0; d < 4; d++)
                        for (int e = 0; e < 4; e++) {
                            int idx = a * 256 + b * 64 + c * 16 + d * 4 + e;
                            bool ok = false;
                            int val[5] = { a, b, c, d, e };
                            for (int i = 0; i < 5 && !ok; i++)
                                for (int j = i + 1; j < 5 && !ok; j++)
                                    for (int k = 0; k < 5 && !ok; k++)
                                        for (int l = k + 1; l < 5 && !ok; l++) {
                                            if (i == k || i == l || j == k || j == l)
                                                continue;             /* disjoint check */
                                            if (val[i] + val[j] == val[k] + val[l])
                                                ok = true;
                                        }
                            good[idx] = ok;
                        }

        /* Precompute transitions between 4‑digit states. */
        for (int s = 0; s < 256; s++) {
            int a = s / 64;          /* high digit (0..3) */
            int rem = s % 64;
            int b = rem / 16;
            rem %= 16;
            int c = rem / 4;
            int d = rem % 4;

            for (int x = 0; x < 4; x++) {
                int idx5 = a * 256 + b * 64 + c * 16 + d * 4 + x;
                if (good[idx5]) {
                    int newstate = b * 64 + c * 16 + d * 4 + x;
                    trans[s][x] = (int16_t)newstate;
                } else
                    trans[s][x] = -1;
            }
        }
        init = true;
    }

    static seq_t cur[256];
    static seq_t nxt[256];

    /* Base case: all 4‑digit prefixes are allowed. */
    for (int s = 0; s < 256; s++)
        cur[s] = 1;

    /* Extend the array one element at a time until length L. */
    for (idx_t len = 5; len <= L; len++) {
        for (int s = 0; s < 256; s++)
            nxt[s] = 0;
        for (int s = 0; s < 256; s++) {
            seq_t v = cur[s];
            if (v == 0)
                continue;
            for (int x = 0; x < 4; x++) {
                int16_t t = trans[s][x];
                if (t >= 0)
                    nxt[t] += v;
            }
        }
        for (int s = 0; s < 256; s++)
            cur[s] = nxt[s];
    }

    /* Sum all ending states to obtain the total count. */
    seq_t total = 0;
    for (int s = 0; s < 256; s++)
        total += cur[s];
    return total;
}
