/*
 * A268885  Number of nX7 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A268885
   Number of n×7 binary arrays with exactly one horizontally or
   antidiagonally adjacent pair of 1's.  The algorithm uses a row‑by‑row
   dynamic programming with 128 possible row patterns.  Only states with
   0 or 1 accumulated pairs are kept, which allows an O(n) solution
   (≈ 3.3×10⁷ transitions for n = 10 000).  The computation uses
   seq_t for arbitrary‑precision; all intermediate values are kept in
   seq_t to avoid overflow. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute horizontal adjacency count for row mask */
static unsigned char horiz[128];
/* Compute anti‑diagonal adjacency between two masks */
static unsigned char anti[128][128];

/* Ensure lookup tables are built once */
static bool tables_built = false;
static void build_tables(void)
{
    if (tables_built) return;
    /* Horizontal pairs */
    for (int mask = 0; mask < 128; ++mask) {
        unsigned char cnt = 0;
        for (int j = 0; j < 6; ++j)
            if ( ( (mask >> j)  & 1u) && ( (mask >> (j+1)) & 1u) )
                ++cnt;
        horiz[mask] = cnt;
    }
    /* Anti‑diagonal pairs between prev and cur */
    for (int prev = 0; prev < 128; ++prev) {
        for (int cur = 0; cur < 128; ++cur) {
            unsigned char cnt = 0;
            for (int j = 0; j < 6; ++j)
                if ( ( (prev >> j) & 1u) && ( (cur >> (j+1)) & 1u) )
                    ++cnt;
            anti[prev][cur] = cnt;
        }
    }
    tables_built = true;
}

seq_t a(idx_t n)
{
    build_tables();

    /* dp_prev[k][mask] : number of ways after processed rows,
       with k (0 or 1) pairs counted so far, ending with row mask.   */
    seq_t dp_prev[2][128] = {{0}};
    seq_t dp_curr[2][128] = {{0}};

    /* Initialize with the first row */
    for (int mask = 0; mask < 128; ++mask) {
        unsigned char hp = horiz[mask];
        if (hp <= 1)
            dp_prev[hp][mask] = (seq_t)1;
    }

    /* Process remaining rows (if any) */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero dp_curr */
        for (int k = 0; k < 2; ++k)
            for (int m = 0; m < 128; ++m)
                dp_curr[k][m] = (seq_t)0;

        for (int prev = 0; prev < 128; ++prev) {
            seq_t val0 = dp_prev[0][prev];
            seq_t val1 = dp_prev[1][prev];
            if (!val0 && !val1) continue;

            for (int cur = 0; cur < 128; ++cur) {
                unsigned char h = horiz[cur];
                unsigned char a = anti[prev][cur];
                unsigned char tot = h + a;
                /* Transition from state with 0 pairs so far */
                if (val0) {
                    unsigned char newk = tot;
                    if (newk <= 1)
                        dp_curr[newk][cur] += val0;
                }
                /* Transition from state with 1 pair so far */
                if (val1) {
                    unsigned char newk = 1 + tot;
                    if (newk <= 1)
                        dp_curr[newk][cur] += val1;
                }
            }
        }
        /* Swap dp_prev and dp_curr */
        for (int k = 0; k < 2; ++k)
            for (int m = 0; m < 128; ++m)
                dp_prev[k][m] = dp_curr[k][m];
    }

    /* Sum all ways with exactly one pair at the end */
    seq_t ans = (seq_t)0;
    for (int mask = 0; mask < 128; ++mask)
        ans += dp_prev[1][mask];

    return ans;
}
