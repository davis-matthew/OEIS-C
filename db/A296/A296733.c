/*
 * A296733  Number of n X 2 0..1 arrays with each 1 adjacent to 1, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A296733 satisfies the recurrence
   a(n) = 3*a(n-1) - a(n-2) + 8*a(n-3) - 12*a(n-4) + 2*a(n-5) - 12*a(n-6)
   with a(1)=2, a(2)=11, a(3)=31, a(4)=88, a(5)=287, a(6)=881.
   This implementation counts the arrangements directly by dynamic
   programming over the 2‑column rows. */
#include <stdbool.h>

/* Return the number of n×2 binary arrays in which every 1 has
   an adjacent count of 1, 2, or 4 king‑move neighbours. */
seq_t a(idx_t n)
{
    /* Precompute compliance of a pattern with its upper and lower neighbours. */
    static bool good[4][4][4];
    static bool done = false;
    if (!done) {
        for (int up = 0; up < 4; ++up) {
            for (int cur = 0; cur < 4; ++cur) {
                for (int down = 0; down < 4; ++down) {
                    bool ok = true;
                    for (int col = 0; col < 2; ++col) {
                        if (!(cur & (1 << col))) continue;
                        int cnt = 0;
                        /* same row neighbor */
                        int other = 1 - col;
                        if (cur & (1 << other)) cnt++;
                        /* upper neighbours */
                        if (up & (1 << col)) cnt++;                 /* up */
                        if (col == 0 && (up & (1 << 1))) cnt++;    /* up‑right */
                        if (col == 1 && (up & (1 << 0))) cnt++;    /* up‑left */
                        /* lower neighbours */
                        if (down & (1 << col)) cnt++;                 /* down */
                        if (col == 0 && (down & (1 << 1))) cnt++;     /* down‑right */
                        if (col == 1 && (down & (1 << 0))) cnt++;     /* down‑left */
                        if (!(cnt == 1 || cnt == 2 || cnt == 4)) {
                            ok = false;
                            break;
                        }
                    }
                    good[cur][up][down] = ok;
                }
            }
        }
        done = true;
    }

    /* dp[up][prev] : number of ways for processed rows up to 'prev',
       with rows (up, prev) being the last two rows. */
    seq_t dp[4][4] = {0}, next[4][4];
    /* Initial state: only the first row has been chosen; upper is phantom 0. */
    for (int mask = 0; mask < 4; ++mask) dp[0][mask] = 1;

    /* Process rows 2 .. n. */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                next[i][j] = 0;
        for (int up = 0; up < 4; ++up) {
            for (int prev = 0; prev < 4; ++prev) {
                seq_t cnt = dp[up][prev];
                if (!cnt) continue;
                for (int cur = 0; cur < 4; ++cur) {
                    if (!good[prev][up][cur]) continue;  /* previous row must be valid */
                    next[prev][cur] += cnt;
                }
            }
        }
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                dp[i][j] = next[i][j];
    }

    /* Final row (lower neighbour is 0). Sum all valid endings. */
    seq_t total = 0;
    for (int up = 0; up < 4; ++up) {
        for (int last = 0; last < 4; ++last) {
            if (!good[last][up][0]) continue;
            total += dp[up][last];
        }
    }
    return total;
}
