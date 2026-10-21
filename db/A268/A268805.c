/*
 * A268805  Number of nX4 0..2 arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A268805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268805
 * Number of n×4 arrays over {0,1,2} with at most one adjacent sum‑2 pair
 * (horizontal, vertical or anti‑diagonal).  Computed by dynamic programming
 * over the 3⁴ possible rows.  Initial values agree with the OEIS list:
 * a(1)=60, a(2)=290, a(3)=1202, …
 */

#include <stdbool.h>
#include <stdint.h>

static int inside_s[81];               /* sum‑2 edges inside a row (horizontal)   */
static uint8_t between_s[81][81];     /* sum‑2 edges between two rows
                                        (vertical + anti‑diagonal)                */

/* Build the tables once.  Called from a() */
static void build_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Pre‑compute the base‑3 digits of each row */
    int digits[81][4];
    for (int r = 0; r < 81; ++r) {
        int t = r;
        for (int c = 0; c < 4; ++c) {
            digits[r][c] = t % 3;
            t /= 3;
        }
    }

    /* Inside‑row edges (horizontal only) */
    for (int r = 0; r < 81; ++r) {
        int s = 0;
        for (int c = 0; c < 3; ++c)
            if (digits[r][c] + digits[r][c + 1] == 2) ++s;
        inside_s[r] = s;
    }

    /* Between‑row edges (vertical + anti‑diagonal) */
    for (int i = 0; i < 81; ++i) {
        for (int j = 0; j < 81; ++j) {
            int k = 0;
            /* vertical edges */
            for (int c = 0; c < 4; ++c)
                if (digits[i][c] + digits[j][c] == 2) ++k;
            /* anti‑diagonal edges (i,c) ↔ (j,c‑1) for c=1..3 */
            for (int c = 1; c < 4; ++c)
                if (digits[i][c] + digits[j][c - 1] == 2) ++k;
            between_s[i][j] = (uint8_t)k;
        }
    }
}

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but harmless */

    build_tables();              /* ensure tables are ready */

    /* dp0[r] – ways ending with row r, no sum‑2 used yet
       dp1[r] – ways ending with row r, one sum‑2 already used   */
    seq_t dp0[81] = {0}, dp1[81] = {0};

    /* First row */
    for (int r = 0; r < 81; ++r) {
        int s = inside_s[r];
        if (s == 0) dp0[r] = 1;
        else if (s == 1) dp1[r] = 1;
        /* s > 1 is impossible → skip */
    }

    /* Process remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        seq_t ndp0[81] = {0}, ndp1[81] = {0};

        for (int i = 0; i < 81; ++i) {
            seq_t v0 = dp0[i];
            seq_t v1 = dp1[i];
            if (v0 == 0 && v1 == 0) continue;

            for (int j = 0; j < 81; ++j) {
                int sj = inside_s[j];
                if (sj > 1) continue;        /* row with >1 internal sum‑2 impossible */

                uint8_t k = between_s[i][j];   /* new edges between i and j */

                /* State with no sum‑2 used so far */
                if (v0) {
                    int total = sj + k;
                    if (total == 0) ndp0[j] += v0;
                    else if (total == 1) ndp1[j] += v0;
                }

                /* State with one sum‑2 already used */
                if (v1) {
                    if (sj == 0 && k == 0) ndp1[j] += v1;
                }
            }
        }

        /* Update dp arrays for next iteration */
        for (int p = 0; p < 81; ++p) {
            dp0[p] = ndp0[p];
            dp1[p] = ndp1[p];
        }
    }

    /* Sum all valid endings */
    seq_t total = 0;
    for (int r = 0; r < 81; ++r) {
        total += dp0[r];
        total += dp1[r];
    }
    return total;
}
