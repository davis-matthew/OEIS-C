/*
 * A283952  Number of 3Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A283952:
    Number of 3×n binary arrays in which all 1’s have at most three
    horizontal, diagonal and antidiagonal neighbors, except for
    exactly one 1 that may have more than three neighbors.
    Computed by dynamic programming over column patterns.  */

#include <stdbool.h>
#include <stddef.h>

enum { STATES = 8 };          /* 3‑row column pattern as 3‑bit mask */

/* Pre‑computed number of violating 1’s in the middle column of a
 * 3×3 block with left column = prev, middle = curr, right = next. */
static unsigned char viol[STATES][STATES][STATES];
static bool viol_inited = false;

static void init_viol(void)
{
    if (viol_inited) return;

    for (int prev = 0; prev < STATES; ++prev) {
        for (int curr = 0; curr < STATES; ++curr) {
            for (int next = 0; next < STATES; ++next) {
                unsigned char v = 0;
                for (int r = 0; r < 3; ++r) {
                    if (((curr >> r) & 1) == 0) continue;  /* only for 1‑cells */

                    int neigh = 0;
                    /* horizontal neighbours (same row) */
                    neigh += (prev >> r) & 1;
                    neigh += (next >> r) & 1;
                    /* diagonal neighbours */
                    if (r > 0) {
                        neigh += (prev >> (r - 1)) & 1;
                        neigh += (next >> (r - 1)) & 1;
                    }
                    if (r < 2) {
                        neigh += (prev >> (r + 1)) & 1;
                        neigh += (next >> (r + 1)) & 1;
                    }
                    if (neigh > 3) ++v;
                }
                viol[prev][curr][next] = v;
            }
        }
    }
    viol_inited = true;
}

seq_t a(idx_t n)
{
    init_viol();

    /* dp[prev][curr][bad] – number of ways for the processed prefix
       ending with columns prev and curr, where bad∈{0,1} is whether
       a violating 1 has already appeared. */
    seq_t dp[STATES][STATES][2];
    seq_t ndp[STATES][STATES][2];

    /* initialise dp for the first column (prev = all‑zero column) */
    for (int p = 0; p < STATES; ++p)
        for (int c = 0; c < STATES; ++c)
            for (int b = 0; b < 2; ++b)
                dp[p][c][b] = 0;
    for (int cur = 0; cur < STATES; ++cur)
        dp[0][cur][0] = 1;

    /* process columns 2 .. n */
    for (idx_t col = 2; col <= n; ++col) {
        /* clear ndp */
        for (int p = 0; p < STATES; ++p)
            for (int c = 0; c < STATES; ++c)
                for (int b = 0; b < 2; ++b)
                    ndp[p][c][b] = 0;

        for (int prev = 0; prev < STATES; ++prev) {
            for (int curr = 0; curr < STATES; ++curr) {
                for (int bad = 0; bad < 2; ++bad) {
                    seq_t cnt = dp[prev][curr][bad];
                    if (cnt == 0) continue;
                    for (int next = 0; next < STATES; ++next) {
                        unsigned char v = viol[prev][curr][next];
                        int nbad = bad + v;
                        if (nbad > 1) continue;
                        ndp[curr][next][nbad] = ndp[curr][next][nbad] + cnt;
                    }
                }
            }
        }
        /* move ndp → dp for next iteration */
        for (int p = 0; p < STATES; ++p)
            for (int c = 0; c < STATES; ++c)
                for (int b = 0; b < 2; ++b)
                    dp[p][c][b] = ndp[p][c][b];
    }

    /* final column: add right boundary (next = all zeros) */
    seq_t ans = 0;
    for (int prev = 0; prev < STATES; ++prev) {
        for (int curr = 0; curr < STATES; ++curr) {
            for (int bad = 0; bad < 2; ++bad) {
                seq_t cnt = dp[prev][curr][bad];
                if (cnt == 0) continue;
                unsigned char v = viol[prev][curr][0];
                if (bad + v == 1)
                    ans = ans + cnt;
            }
        }
    }
    return ans;
}
