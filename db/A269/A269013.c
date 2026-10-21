/*
 * A269013  Number of 3 X n binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×n binary arrays containing exactly one pair
 *            of horizontally, diagonally or antidiagonally adjacent 1's. The
 *            adjacency is only between cells in columns j and j+1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements a length‑n DP that counts these arrays
   for any n ≥ 1.  It uses only integer arithmetic that is valid for
   any seq_t type (C or C++), handles large values via arbitrary‑precision
   seq_t when required, and runs in O(n) time with a tiny constant. */

#include <stdint.h>
#include <stdbool.h>

static const uint8_t popcnt3[8] = {
    0, 1, 1, 2, 1, 2, 2, 3
};

/* Precomputed pair count for every ordered pair of 3‑bit column states.
   pair_cnt[s][t] = number of adjacent 1‑1 pairs formed by columns s (left)
   and t (right).  Value is 0..6. */
static uint8_t pair_cnt[8][8];
static bool pair_cnt_inited = false;

static void init_pair_cnt(void)
{
    if (pair_cnt_inited) return;
    for (int s = 0; s < 8; ++s) {
        int a = s & 1;
        int b = (s >> 1) & 1;
        int c = (s >> 2) & 1;
        for (int t = 0; t < 8; ++t) {
            int d = t & 1;
            int e = (t >> 1) & 1;
            int f = (t >> 2) & 1;
            uint8_t cnt = 0;
            cnt += (a & d);            /* horizontal 0‑0 */
            cnt += (b & e);            /* horizontal 1‑1 */
            cnt += (c & f);            /* horizontal 2‑2 */
            cnt += (a & e);            /* diagonal 0‑1 */
            cnt += (b & f);            /* diagonal 1‑2 */
            cnt += (b & d);            /* antidiagonal 1‑0 */
            cnt += (c & e);            /* antidiagonal 2‑1 */
            pair_cnt[s][t] = cnt;
        }
    }
    pair_cnt_inited = true;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;    /* a(1)=0, a(0) irrelevant */

    init_pair_cnt();

    /* dp0[s] = number of length‑k sequences ending with column state s
       having 0 adjacent‑1‑pairs so far.
       dp1[s] = number of ending state s with exactly 1 adjacent‑1‑pair. */
    seq_t dp0[8] = {0};
    seq_t dp1[8] = {0};
    seq_t new0[8] = {0};
    seq_t new1[8] = {0};

    for (int s = 0; s < 8; ++s) dp0[s] = (seq_t)1;  /* first column arbitrary */

    for (idx_t step = 1; step < n; ++step) {
        /* zero new arrays */
        for (int i = 0; i < 8; ++i) new0[i] = new1[i] = (seq_t)0;

        for (int s = 0; s < 8; ++s) {
            seq_t d0 = dp0[s];
            seq_t d1 = dp1[s];
            if (!d0 && !d1) continue;
            for (int t = 0; t < 8; ++t) {
                uint8_t c = pair_cnt[s][t];
                if (c == 0) {
                    if (d0) new0[t] += d0;
                    if (d1) new1[t] += d1;
                } else if (c == 1) {
                    if (d0) new1[t] += d0;
                    /* d1 cannot transition because would exceed one pair */
                }
                /* c >= 2 : transition would produce >1 pair, ignore */
            }
        }
        /* copy new arrays back to dp for next iteration */
        for (int i = 0; i < 8; ++i) {
            dp0[i] = new0[i];
            dp1[i] = new1[i];
        }
    }

    /* sum over all ending states with exactly one pair */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 8; ++i) ans += dp1[i];
    return ans;
}
