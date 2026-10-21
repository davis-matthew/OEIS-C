/*
 * A250383  Number of length n+3 0..4 arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by a 125‑state DP with 6 forbidden checks per
 *            transition
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 29, u128 n <= 63,
 *            big any n
 * Helpers:   idx_from
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A250383:  
 *  a(n) = number of length n+3 arrays with entries 0..4 such that for every four
 *  consecutive terms the maximum of any two terms is not equal to the minimum
 *  of the remaining two terms.
 *
 *  The sequence is computed by dynamic programming over the last three terms.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/idx_from.h"

#define STATE_COUNT 125 /* 5^3 */

static bool good[STATE_COUNT][5];
static unsigned char x_of[STATE_COUNT];
static unsigned char y_of[STATE_COUNT];
static unsigned char z_of[STATE_COUNT];

static void init_tables(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    for (int a = 0; a < 5; ++a)
        for (int b = 0; b < 5; ++b)
            for (int c = 0; c < 5; ++c) {
                int idx = a * 25 + b * 5 + c;
                x_of[idx] = (unsigned char)a;
                y_of[idx] = (unsigned char)b;
                z_of[idx] = (unsigned char)c;

                for (int w = 0; w < 5; ++w) {
                    int t[4] = {a, b, c, w};
                    bool forbidden = false;
                    /* enumerate all unordered pairs (i<j) */
                    for (int i = 0; i < 4 && !forbidden; ++i)
                        for (int j = i + 1; j < 4 && !forbidden; ++j) {
                            int max1 = t[i] > t[j] ? t[i] : t[j];
                            /* find the two remaining indices */
                            int k = -1, l = -1;
                            for (int k0 = 0; k0 < 4; ++k0)
                                if (k0 != i && k0 != j) {
                                    if (k == -1) k = k0;
                                    else l = k0;
                                }
                            int min2 = t[k] < t[l] ? t[k] : t[l];
                            if (max1 == min2) forbidden = true;
                        }
                    good[idx][w] = !forbidden;
                }
            }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_tables();

    /* length of the array */
    const idx_t L = n + 3;
    /* number of steps: we have two more positions after the initial 3 */
    const idx_t steps = L - 3;

    seq_t cur[STATE_COUNT];
    seq_t nxt[STATE_COUNT];

    /* all 3‑tuples are possible starting states */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = 1;

    for (idx_t s = 0; s < steps; ++s) {
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = 0;

        for (int idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t cnt = cur[idx];
            if (cnt == 0) continue;
            int a = x_of[idx];
            int b = y_of[idx];
            int c = z_of[idx];
            for (int w = 0; w < 5; ++w) {
                if (!good[idx][w]) continue;
                int next_idx = idx_from(b, c, w);
                nxt[next_idx] += cnt;
            }
        }
        /* copy next to cur */
        for (int i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i];
    }

    seq_t result = 0;
    for (int i = 0; i < STATE_COUNT; ++i) result += cur[i];
    return result;
}
