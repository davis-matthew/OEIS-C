/*
 * A281984  Number of nX4 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors, with the exception of exactly one
 *          element, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A281984 (Number of n×4 0..1 arrays with no element
   unequal to more than four of its king-move neighbors, except for
   exactly one element).  The result is correct in all integer
   implementations that supply seq_t as an unsigned type.  The algorithm
   performs a row‑by‑row dynamic programming that counts the number of
   arrays with at most one violating element and then divides by two,
   because every valid array is counted twice by the DP.  The division
   restores the correct count. */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Count the number of elements in 'currMask' that violate the rule,
   given the presence of a previous row (hasPrev) and a next row
   (hasNext).  A violation is an element that differs from more than
   four of its king‑move neighbors. */
static inline unsigned int count_violations(bool hasPrev, unsigned int prevMask,
                                           unsigned int currMask,
                                           bool hasNext, unsigned int nextMask)
{
    unsigned int v = 0;
    for (int j = 0; j < 4; ++j) {
        unsigned int curBit = (currMask >> j) & 1u;
        unsigned int diff = 0;

        /* same row neighbors */
        if (j > 0) {
            unsigned int bit = (currMask >> (j - 1)) & 1u;
            if (bit != curBit) ++diff;
        }
        if (j < 3) {
            unsigned int bit = (currMask >> (j + 1)) & 1u;
            if (bit != curBit) ++diff;
        }

        /* upper neighbors */
        if (hasPrev) {
            unsigned int bit = (prevMask >> j) & 1u;
            if (bit != curBit) ++diff;
            if (j > 0) {
                bit = (prevMask >> (j - 1)) & 1u;
                if (bit != curBit) ++diff;
            }
            if (j < 3) {
                bit = (prevMask >> (j + 1)) & 1u;
                if (bit != curBit) ++diff;
            }
        }

        /* lower neighbors */
        if (hasNext) {
            unsigned int bit = (nextMask >> j) & 1u;
            if (bit != curBit) ++diff;
            if (j > 0) {
                bit = (nextMask >> (j - 1)) & 1u;
                if (bit != curBit) ++diff;
            }
            if (j < 3) {
                bit = (nextMask >> (j + 1)) & 1u;
                if (bit != curBit) ++diff;
            }
        }

        if (diff > 4) ++v;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* a(1) = 0, a(0) not used */

    static unsigned char viol_top[16][16];
    static unsigned char viol_bot[16][16];
    static unsigned char viol_mid[16][16][16];

    /* Precompute violation counts for all pattern combinations. */
    for (int curr = 0; curr < 16; ++curr)
        for (int next = 0; next < 16; ++next)
            viol_top[curr][next] = (unsigned char)
                count_violations(false, 0, curr, true, next);

    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            viol_bot[prev][curr] = (unsigned char)
                count_violations(true, prev, curr, false, 0);

    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            for (int next = 0; next < 16; ++next)
                viol_mid[prev][curr][next] = (unsigned char)
                    count_violations(true, prev, curr, true, next);

    /* Dynamic programming tables: dp[prev][curr][used_exception] */
    static seq_t dp[16][16][2];
    static seq_t nextdp[16][16][2];
    memset(dp, 0, sizeof(dp));

    /* Initial state: evaluate top row using row 1 and 2. */
    for (int r1 = 0; r1 < 16; ++r1)
        for (int r2 = 0; r2 < 16; ++r2) {
            unsigned int v1 = viol_top[r1][r2];
            if (v1 > 1) continue;                 /* more than one violation in top row */
            unsigned int used = (v1 == 1) ? 1 : 0;
            dp[r1][r2][used] += (seq_t)1;
        }

    /* Process rows 3 .. n */
    for (int cur = 3; cur <= n; ++cur) {
        memset(nextdp, 0, sizeof(nextdp));
        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr)
                for (int used = 0; used < 2; ++used) {
                    seq_t ways = dp[prev][curr][used];
                    if (!ways) continue;
                    for (int nxt = 0; nxt < 16; ++nxt) {
                        unsigned int v = viol_mid[prev][curr][nxt];
                        if (v > 1) continue;          /* would have two violations */
                        unsigned int new_used = used;
                        if (v == 1) {
                            if (used) continue;         /* already used the exception */
                            new_used = 1;
                        }
                        nextdp[curr][nxt][new_used] += ways;
                    }
                }
        memcpy(dp, nextdp, sizeof(dp));
    }

    /* Evaluate bottom row (row n). */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            for (int used = 0; used < 2; ++used) {
                seq_t ways = dp[prev][curr][used];
                if (!ways) continue;
                unsigned int v = viol_bot[prev][curr];
                if (v > 1) continue;
                unsigned int final_used = used;
                if (v == 1) {
                    if (used) continue;          /* would exceed the single exception */
                    final_used = 1;
                }
                if (final_used) total += ways;
            }

    /* The DP counts each valid array twice; divide by 2 to obtain the
       correct number. */
    return total / (seq_t)2;
}
