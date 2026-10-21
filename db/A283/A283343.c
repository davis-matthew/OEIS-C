/*
 * A283343  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..1 arrays of size n×4 in which none of the
 *            1‑cells has more than two of its horizontal, vertical, or
 *            antidiagonal neighbors 1, except for exactly one cell
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Precompute the number of violations in a row *prev* when the two rows
   above and below are given by *prev_prev* and *cur*. */
static unsigned char V[16][16][16];
static bool V_init = false;

void a_init(void)
{
    if (!V_init) {
        for (int pp = 0; pp < 16; ++pp) {
            for (int p  = 0; p  < 16; ++p) {
                for (int c  = 0; c  < 16; ++c) {
                    unsigned char viol = 0;
                    for (int j = 0; j < 4; ++j) {
                        if ((p >> j) & 1) {
                            int nb = 0;
                            /* horizontal */
                            if (j > 0 && (p >> (j-1)) & 1) nb++;
                            if (j < 3 && (p >> (j+1)) & 1) nb++;
                            /* vertical */
                            if ((pp >> j) & 1) nb++;  /* up */
                            if ((c >> j)  & 1) nb++;  /* down */
                            /* antidiagonal (NE‑SW) */
                            if (j < 3 && (pp >> (j+1)) & 1) nb++; /* up‑right */
                            if (j > 0 && (c  >> (j-1)) & 1) nb++; /* down‑left */
                            if (nb > 2) viol++;
                        }
                    }
                    V[pp][p][c] = viol;
                }
            }
        }
        V_init = true;
    }
}

/* DP arrays */
static seq_t dp[512];
static seq_t ndp[512];

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    a_init();                          /* ensure its table exists */

    /* index mapping: key = (prev_prev << 5) | (prev << 1) | viol   */
    for (int i = 0; i < 512; ++i) dp[i] = 0;
    dp[0] = 1;   /* start with two imaginary zero rows */

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < 512; ++i) ndp[i] = 0;
        for (int key = 0; key < 512; ++key) {
            seq_t curcnt = dp[key];
            if (!curcnt) continue;
            int prev_prev = key >> 5;
            int prev      = (key >> 1) & 0xF;
            int viol      = key & 1;

            for (int cur = 0; cur < 16; ++cur) {
                unsigned char vrow = V[prev_prev][prev][cur];
                int newviol = viol + (int)vrow;
                if (newviol > 1) continue;
                int newkey = (prev << 5) | (cur << 1) | newviol;
                ndp[newkey] += curcnt;
            }
        }
        /* swap */
        for (int i = 0; i < 512; ++i) dp[i] = ndp[i];
    }

    /* evaluate the last row (row n-1) using cur = 0 */
    seq_t result = 0;
    for (int key = 0; key < 512; ++key) {
        seq_t curcnt = dp[key];
        if (!curcnt) continue;
        int prev_prev = key >> 5;
        int prev      = (key >> 1) & 0xF;
        int viol      = key & 1;

        unsigned char vrow_last = V[prev_prev][prev][0];
        int final_viol = viol + (int)vrow_last;
        if (final_viol == 1) result += curcnt;
    }

    return result;
}
