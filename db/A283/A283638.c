/*
 * A283638  Number of 5Xn 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 5×n binary arrays where each 1 has at most one
 * 1 among its horizontal, diagonal, and antidiagonal neighbors, *except*
 * exactly two elements may violate this rule.
 *
 * The implementation uses dynamic programming over column triples.
 * For each column vector (5 bits) we consider the 6 possible neighbors
 * that influence the constraints of a cell.  For a fixed triple
 * (prev, curr, next) we pre‑compute how many cells in the middle
 * column violate the rule (`viol[prev][curr][next]`).
 *
 * The DP state is (prev_col, curr_col, viol_count) where
 * viol_count ∈ {0,1,2} is the number of already counted violations.
 * Transition from state (p,c,v) to (c,n,v') adds a new column `n`
 * and sets v' = v + viol[p][c][n] (discarded if v' > 2).
 *
 * After processing all n columns we add a dummy zero column on the
 * right side to validate the last column.  The answer is the sum of
 * all DP states with total violations == 2.
 *
 * Complexity: O(n · 32³) ≈ 1e9 elementary operations for n=10 000,
 * well within the limits in optimized C.  The DP arrays are fixed
 * size 32×32×3 = 3072 entries.
 */

#include <stdint.h>
#include <stddef.h>

static unsigned char viol[32][32][32];
static int viol_init = 0;

static void init_violations(void)
{
    if (viol_init) return;
    for (int p = 0; p < 32; ++p) {
        for (int c = 0; c < 32; ++c) {
            for (int n = 0; n < 32; ++n) {
                unsigned char cnt = 0;
                for (int r = 0; r < 5; ++r) {
                    if (!((c >> r) & 1)) continue;          /* only 1 cells matter */
                    unsigned neigh = 0;
                    /* left same row */
                    neigh += (p >> r) & 1;
                    /* right same row */
                    neigh += (n >> r) & 1;
                    /* diagonal up-left */
                    if (r > 0) neigh += (p >> (r-1)) & 1;
                    /* diagonal down-right */
                    if (r < 4) neigh += (n >> (r+1)) & 1;
                    /* antidiagonal up-right */
                    if (r > 0) neigh += (n >> (r-1)) & 1;
                    /* antidiagonal down-left */
                    if (r < 4) neigh += (p >> (r+1)) & 1;
                    if (neigh > 1) ++cnt;
                }
                viol[p][c][n] = cnt;
            }
        }
    }
    viol_init = 1;
}

static inline size_t idx(int p, int c)
{
    return ((size_t)p << 5) | (size_t)c;
}

/* Main function required by the tests */
seq_t a(idx_t n)
{
    init_violations();

    /* DP arrays: [prev_col][curr_col] for each violation count */
    seq_t dp0[1024] = {0};
    seq_t dp1[1024] = {0};
    seq_t dp2[1024] = {0};
    seq_t ndp0[1024], ndp1[1024], ndp2[1024];

    /* Initialize with column 0 = 0 and first column c1 (any 5‑bit vector) */
    for (int c1 = 0; c1 < 32; ++c1) {
        dp0[idx(0,c1)] = 1;
    }

    /* Process columns 1..n-1 (adding column i+1 each step) */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next arrays */
        for (size_t i = 0; i < 1024; ++i) {
            ndp0[i] = ndp1[i] = ndp2[i] = 0;
        }

        /* transitions from dp0 (v = 0) */
        for (size_t i = 0; i < 1024; ++i) {
            seq_t val = dp0[i];
            if (!val) continue;
            int p = (int)(i >> 5);
            int c = (int)(i & 31);
            for (int ncol = 0; ncol < 32; ++ncol) {
                unsigned char vcnt = viol[p][c][ncol];
                size_t ni = idx(c, ncol);
                if (vcnt == 0) ndp0[ni] += val;
                else if (vcnt == 1) ndp1[ni] += val;
                else if (vcnt == 2) ndp2[ni] += val;
            }
        }

        /* transitions from dp1 (v = 1) */
        for (size_t i = 0; i < 1024; ++i) {
            seq_t val = dp1[i];
            if (!val) continue;
            int p = (int)(i >> 5);
            int c = (int)(i & 31);
            for (int ncol = 0; ncol < 32; ++ncol) {
                unsigned char vcnt = viol[p][c][ncol];
                size_t ni = idx(c, ncol);
                if (vcnt == 0) ndp1[ni] += val;
                else if (vcnt == 1) ndp2[ni] += val;
                /* vcnt == 2 would give total 3, discard */
            }
        }

        /* transitions from dp2 (v = 2) only if no new violation */
        for (size_t i = 0; i < 1024; ++i) {
            seq_t val = dp2[i];
            if (!val) continue;
            int p = (int)(i >> 5);
            int c = (int)(i & 31);
            for (int ncol = 0; ncol < 32; ++ncol) {
                unsigned char vcnt = viol[p][c][ncol];
                if (vcnt == 0) {
                    size_t ni = idx(c, ncol);
                    ndp2[ni] += val;
                }
            }
        }

        /* move next to current for next iteration */
        for (size_t i = 0; i < 1024; ++i) {
            dp0[i] = ndp0[i];
            dp1[i] = ndp1[i];
            dp2[i] = ndp2[i];
        }
    }

    /* Final verification of the last column against a zero column */
    seq_t result = 0;
    for (size_t i = 0; i < 1024; ++i) {
        seq_t val0 = dp0[i];
        seq_t val1 = dp1[i];
        seq_t val2 = dp2[i];
        if (val0 || val1 || val2) {
            int p = (int)(i >> 5);
            int c = (int)(i & 31);
            unsigned char vcnt = viol[p][c][0];
            /* accumulated violations so far = v; final = v + vcnt */
            if (vcnt == 2) result += val0;   /* 0+2=2 */
            if (vcnt == 1) result += val1;   /* 1+1=2 */
            if (vcnt == 0) result += val2;   /* 2+0=2 */
        }
    }
    return result;
}
