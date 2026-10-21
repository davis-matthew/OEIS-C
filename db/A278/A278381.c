/*
 * A278381  Number of nX4 0..1 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly three mistakes.
 * https://oeis.org/A278381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary matrices such that rows and columns,
 *            considered as 4‑bit and n‑bit numbers respectively, are in
 *            lexicographic nondecreasing order but exactly three adjacent
 *            comparisons (among the n−1 row comparisons and the 3 column
 *            comparisons) are violated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Precomputed tables – initialized once. */
    static unsigned char row_bits[16][4];
    static unsigned char transition_table[27][16];
    static unsigned char status_mistakes[27];
    static unsigned char row_gt[16][16];
    static int tables_ready = 0;

    if (!tables_ready) {
        tables_ready = 1;

        /* Bits of each 4‑bit row (bit 0 = most significant). */
        for (int r = 0; r < 16; r++) {
            for (int b = 0; b < 4; b++)
                row_bits[r][b] = (r >> (3 - b)) & 1U;
        }

        /* row_gt[a][b] = 1 if a > b (lexicographic violation) */
        for (int a = 0; a < 16; a++)
            for (int b = 0; b < 16; b++)
                row_gt[a][b] = (a > b) ? 1U : 0U;

        /* Count mistakes for each status (number of 2‑states). */
        for (int s = 0; s < 27; s++) {
            int m = 0;
            int tmp = s;
            for (int p = 0; p < 3; p++) {
                int st = tmp % 3;
                if (st == 2) m++;
                tmp /= 3;
            }
            status_mistakes[s] = (unsigned char)m;
        }

        /* Transition table: next state given current state and new row. */
        for (int s = 0; s < 27; s++) {
            int st0 = s % 3;          /* pair (1,2) */
            int st1 = (s / 3) % 3;    /* pair (2,3) */
            int st2 = (s / 9) % 3;    /* pair (3,4) */
            for (int r = 0; r < 16; r++) {
                int ns0, ns1, ns2;

                /* Pair (1,2) */
                if (st0 == 0) {
                    unsigned int a = row_bits[r][0];
                    unsigned int b = row_bits[r][1];
                    if (a == b) ns0 = 0;
                    else if (a < b) ns0 = 1;
                    else ns0 = 2;
                } else {
                    ns0 = st0;
                }

                /* Pair (2,3) */
                if (st1 == 0) {
                    unsigned int a = row_bits[r][1];
                    unsigned int b = row_bits[r][2];
                    if (a == b) ns1 = 0;
                    else if (a < b) ns1 = 1;
                    else ns1 = 2;
                } else {
                    ns1 = st1;
                }

                /* Pair (3,4) */
                if (st2 == 0) {
                    unsigned int a = row_bits[r][2];
                    unsigned int b = row_bits[r][3];
                    if (a == b) ns2 = 0;
                    else if (a < b) ns2 = 1;
                    else ns2 = 2;
                } else {
                    ns2 = st2;
                }

                transition_table[s][r] = (ns0 + 3 * ns1 + 9 * ns2);
            }
        }
    }

    /* DP arrays: dp[2][row][state][k] where k = number of row violations (<=3). */
    static seq_t dp[2][16][27][4];
    int cur = 0, nxt = 1;

    /* Zero the DP arrays for this call. */
    for (int r = 0; r < 16; r++)
        for (int s = 0; s < 27; s++)
            for (int k = 0; k < 4; k++) {
                dp[cur][r][s][k] = 0;
                dp[nxt][r][s][k] = 0;
            }

    /* Initialize after first row. No row violations yet (k=0). */
    for (int r = 0; r < 16; r++) {
        int s0 = transition_table[0][r];  /* state after first row */
        dp[cur][r][s0][0] += 1;
    }

    /* Process remaining rows. */
    for (idx_t step = 1; step < n; step++) {
        /* Zero next layer. */
        for (int r = 0; r < 16; r++)
            for (int s = 0; s < 27; s++)
                for (int k = 0; k < 4; k++)
                    dp[nxt][r][s][k] = 0;

        /* DP transitions. */
        for (int rprev = 0; rprev < 16; rprev++) {
            for (int s = 0; s < 27; s++) {
                for (int k = 0; k <= 3; k++) {
                    seq_t curval = dp[cur][rprev][s][k];
                    if (!curval)
                        continue;
                    for (int rcurr = 0; rcurr < 16; rcurr++) {
                        int v = row_gt[rprev][rcurr];     /* row violation? */
                        int k2 = k + v;
                        if (k2 > 3)
                            continue;
                        int s2 = transition_table[s][rcurr];
                        dp[nxt][rcurr][s2][k2] += curval;
                    }
                }
            }
        }
        /* Swap layers. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum counts with total mistakes = 3 (row + column). */
    seq_t result = 0;
    for (int r = 0; r < 16; r++)
        for (int s = 0; s < 27; s++) {
            unsigned int cmistakes = status_mistakes[s];
            if (cmistakes > 3)
                continue; /* impossible for total = 3 */
            for (int k = 0; k <= 3; k++) {
                if (k + cmistakes == 3)
                    result += dp[cur][r][s][k];
            }
        }

    return result;
}
