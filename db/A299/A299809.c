/*
 * A299809  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by a row‑by‑row dynamic programming. The DP
 *            iterates over all possible 3‑bit rows (0…7) plus a sentinel
 *            value (8) for the non‑existent rows above the first row and
 *            below the last row. For each pair (previous, current) of rows it
 *            stores the number of ways to reach that configuration,...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Get the value (0 or 1) at column c (0=leftmost) of a 3‑bit row. */
static inline int get_bit(int row, int col) {
    return (row >> (2 - col)) & 1;
}

/* Check that every cell in the centre row satisfies the forbidden
   neighbor count rule, given the rows above and below.  `sentinel`
   denotes a non‑existent row. */
static inline bool ok_row(int centre, int above, int below) {
    const int SENT = 8;              /* 3‑bit rows are 0…7, so 8 is free */
    for (int c = 0; c < 3; ++c) {    /* column index */
        int val = get_bit(centre, c);
        int cnt = 0;

        /* Neighbours from the row above. */
        if (above != SENT) {
            int bit = get_bit(above, c);                 /* up */
            if (bit == val) ++cnt;
            if (c > 0) {                                 /* up‑left */
                bit = get_bit(above, c - 1);
                if (bit == val) ++cnt;
            }
            if (c < 2) {                                 /* up‑right */
                bit = get_bit(above, c + 1);
                if (bit == val) ++cnt;
            }
        }

        /* Neighbours from the same row. */
        if (c > 0) {                                     /* left */
            int bit = get_bit(centre, c - 1);
            if (bit == val) ++cnt;
        }
        if (c < 2) {                                     /* right */
            int bit = get_bit(centre, c + 1);
            if (bit == val) ++cnt;
        }

        /* Neighbours from the row below. */
        if (below != SENT) {
            int bit = get_bit(below, c);                 /* down */
            if (bit == val) ++cnt;
            if (c > 0) {                                 /* down‑left */
                bit = get_bit(below, c - 1);
                if (bit == val) ++cnt;
            }
            if (c < 2) {                                 /* down‑right */
                bit = get_bit(below, c + 1);
                if (bit == val) ++cnt;
            }
        }

        /* Forbidden counts are 2 and 7. */
        if (cnt == 2 || cnt == 7) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    enum { ROWS = 8, SENT = 8, STATE = 9 };   /* 3‑bit rows 0…7 + sentinel */
    seq_t dp[STATE][STATE];
    seq_t next[STATE][STATE];

    /* Initialise dp to zero. */
    for (int i = 0; i < STATE; ++i)
        for (int j = 0; j < STATE; ++j)
            dp[i][j] = 0;

    /* First row: upper‑left (column 0) must be 0. */
    for (int r = 0; r < ROWS; ++r) {
        if (((r >> 2) & 1) == 0) {          /* leftmost bit is 0 */
            dp[SENT][r] = 1;
        }
    }

    /* Process rows 2 … n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Clear next DP table. */
        for (int i = 0; i < STATE; ++i)
            for (int j = 0; j < STATE; ++j)
                next[i][j] = 0;

        for (int above = 0; above < STATE; ++above) {
            for (int centre = 0; centre < STATE; ++centre) {
                seq_t curcnt = dp[above][centre];
                if (curcnt == 0) continue;

                for (int below = 0; below < ROWS; ++below) {
                    if (!ok_row(centre, above, below)) continue;
                    next[centre][below] += curcnt;
                }
            }
        }

        /* Move to next iteration. */
        for (int i = 0; i < STATE; ++i)
            for (int j = 0; j < STATE; ++j)
                dp[i][j] = next[i][j];
    }

    /* Final row: validate with below sentinel. */
    seq_t ans = 0;
    for (int above = 0; above < STATE; ++above) {
        for (int centre = 0; centre < STATE; ++centre) {
            seq_t curcnt = dp[above][centre];
            if (curcnt == 0) continue;

            if (ok_row(centre, above, SENT)) {
                ans += curcnt;
            }
        }
    }
    return ans;
}
