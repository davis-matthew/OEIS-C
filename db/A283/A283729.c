/*
 * A283729  Number of 4Xn 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This DP counts 4×n binary arrays with exactly one 1 cell having
 *            more than two of its horizontal, diagonal and antidiagonal
 *            neighbors equal to 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

/* Global lookup table: number of violating cells in the middle column
   when neighboring columns are prev, curr and next. */
static signed char violation[16][16][16];
static int violation_table_initialized = 0;

/* Build the table once. */
static void init_violation_table(void)
{
    if (violation_table_initialized) return;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            for (int next = 0; next < 16; ++next) {
                int cnt = 0;
                for (int r = 0; r < 4; ++r) {
                    int bit = (curr >> r) & 1;
                    if (!bit) continue;          /* only 1‑cells can violate */
                    int neigh = 0;
                    /* horizontal neighbours */
                    neigh += (prev >> r) & 1;     /* left */
                    neigh += (next >> r) & 1;     /* right */
                    /* diagonal and antidiagonal neighbours */
                    if (r > 0)   neigh += (prev >> (r-1)) & 1;   /* up‑left */
                    if (r < 3)   neigh += (prev >> (r+1)) & 1;   /* down‑left */
                    if (r > 0)   neigh += (next >> (r-1)) & 1;   /* up‑right */
                    if (r < 3)   neigh += (next >> (r+1)) & 1;   /* down‑right */
                    if (neigh > 2) ++cnt;
                }
                violation[prev][curr][next] = (signed char)cnt;
            }
        }
    }
    violation_table_initialized = 1;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n<1 */

    init_violation_table();

    /* dp[prev][curr][used] counts partial arrays where
       'prev' is the column before 'curr', and 'used' indicates
       whether the single allowed violation has occurred. */
    static seq_t dp[16][16][2];
    static seq_t dp_next[16][16][2];

    memset(dp, 0, sizeof(dp));
    dp[0][0][0] = (seq_t)1;               /* start with empty left boundary */

    for (idx_t step = 0; step <= n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));

        int next_vals[16];
        int next_cnt;
        if (step == n) {                  /* dummy right boundary */
            next_vals[0] = 0;
            next_cnt = 1;
        } else {
            for (int v = 0; v < 16; ++v) next_vals[v] = v;
            next_cnt = 16;
        }

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                for (int used = 0; used < 2; ++used) {
                    seq_t ways = dp[prev][curr][used];
                    if (!ways) continue;
                    for (int k = 0; k < next_cnt; ++k) {
                        int next = next_vals[k];
                        int vcnt = violation[prev][curr][next];
                        int newUsed;
                        if (vcnt == 0) {
                            newUsed = used;
                        } else if (vcnt == 1) {
                            if (used) continue;  /* violation already used */
                            newUsed = 1;
                        } else {
                            continue;             /* more than one violation */
                        }
                        dp_next[curr][next][newUsed] += ways;
                    }
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* After the final transition the right column is dummy 0. */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev)
        result += dp[prev][0][1];           /* collect states with exactly one violation */

    return result;
}
