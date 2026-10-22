/*
 * A345499  Numbers that are the sum of nine squares in two or more ways.
 * https://oeis.org/A345499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are expressed as a sum of nine squares in two or more distinct ways. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

#define MAXS 50000   /* upper bound for precomputed sums */

static size_t seq_len = 0;
static seq_t *seq = NULL;
static unsigned char dp[10][MAXS + 1] = {0};
static bool built = false;

static void build_sequence(void)
{
    if (built) return;
    built = true;

    dp[0][0] = 1;
    unsigned int maxV = (unsigned int)int_sqrt((idx_t)MAXS);

    for (unsigned int v = 1; v <= maxV; ++v) {
        unsigned int val = v * v;
        for (int k = 1; k <= 9; ++k) {
            for (unsigned int s = val; s <= MAXS; ++s) {
                unsigned char add = dp[k - 1][s - val];
                if (!add) continue;
                unsigned char cur = dp[k][s];
                if (cur >= 2) continue;
                unsigned char nval = cur + add;
                if (nval > 2) nval = 2;
                dp[k][s] = nval;
            }
        }
    }

    seq = (seq_t *)malloc((MAXS + 1) * sizeof(seq_t));
    if (!seq) return;  /* allocation failure, but unlikely */

    for (unsigned int s = 1; s <= MAXS; ++s) {
        if (dp[9][s] >= 2) {
            seq[seq_len++] = (seq_t)s;
        }
    }
}

seq_t a(idx_t n)
{
    build_sequence();
    if (n == 0 || (size_t)n > seq_len) {
        return (seq_t)0;  /* out of bounds */
    }
    return seq[(size_t)(n - 1)];
}
