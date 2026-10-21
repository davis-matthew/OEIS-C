/*
 * A277446  Longest partition of the binary digits of n read from left to
 *          right into contiguous blocks so that the value of a block divides
 *          the value of its successor and with no leading zeros in the
 *          blocks.
 * https://oeis.org/A277446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation for A277446: longest partition of the binary digits of n into
 * contiguous blocks so that each block starts with a 1, all bits are used,
 * and the integer value of each block divides the value of its successor.
 * The function returns the maximum number of blocks achievable. */

#include <stdbool.h>

typedef struct {
    int start;       /* start bit index (MSB side) */
    int end;         /* end bit index (inclusive) */
    seq_t val;       /* integer value of bits[start..end] */
} Interval;

/* Recursive helper with memoisation;
 * dp[idx] == 0  -> not computed or no valid path to the end
 * dp[idx] > 0  -> maximal number of blocks from this interval to the end */
static int best_path(int idx, const Interval *intervals, int num, int len, int *dp)
{
    int cached = dp[idx];
    if (cached != -1) return cached;

    /* If this interval ends at the last bit, it is a valid chain of one block */
    if (intervals[idx].end == len - 1) {
        dp[idx] = 1;
        return 1;
    }

    int best = 0;  /* 0 indicates no valid continuation */

    /* Attempt to extend the chain by a successor interval that starts
     * immediately after this block */
    for (int j = 0; j < num; ++j) {
        if (intervals[j].start != intervals[idx].end + 1)
            continue;  /* not an adjacent successor */

        /* Divisibility test: current block value must divide successor value */
        if (intervals[idx].val == 0) continue;  /* impossible, but safe */
        if (intervals[j].val % intervals[idx].val != 0)
            continue;

        int cand = best_path(j, intervals, num, len, dp);
        if (cand > 0 && cand + 1 > best)
            best = cand + 1;
    }

    dp[idx] = best;   /* best may stay 0 ==> no valid path to end */
    return best;
}

seq_t a(idx_t n)
{
    /* Handle the degenerate case */
    if (n == 0) return (seq_t)0;

    /* Determine number of bits in n */
    idx_t t = n;
    int len = 0;
    while (t) { t >>= 1; ++len; }

    /* Extract bits into an array MSB first */
    unsigned char bits[64];
    t = n;
    for (int i = 0; i < len; ++i) {
        int shift = len - 1 - i;
        bits[i] = (t >> shift) & 1;
    }

    /* Build all valid intervals */
    Interval intervals[2100];  /* enough for len <= 64 (64*65/2=2080) */
    int num = 0;
    for (int s = 0; s < len; ++s) {
        if (bits[s] == 0) continue;   /* starts must be 1 */
        seq_t val = 0;
        for (int e = s; e < len; ++e) {
            val = (val << 1) | (seq_t)bits[e];
            if (e == len - 1 || bits[e + 1] == 1) {
                intervals[num].start = s;
                intervals[num].end   = e;
                intervals[num].val   = val;
                ++num;
            }
        }
    }

    /* Memoisation array */
    int dp[2100];
    for (int i = 0; i < num; ++i) dp[i] = -1;

    /* Evaluate all intervals that start at bit 0 */
    int answer = 0;
    for (int i = 0; i < num; ++i) {
        if (intervals[i].start == 0) {
            int cur = best_path(i, intervals, num, len, dp);
            if (cur > answer) answer = cur;
        }
    }

    return (seq_t)answer;
}
