/*
 * A250727  Number of (n+1) X (6+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over transitions of
 *            7‑bit rows. Each array is an (n+1)×7 0/1 matrix. For every pair
 *            of consecutive rows (prev, curr) we check: - for all j=1..6,
 *            (curr[j]+curr[j-1]) ≥ (prev[j]+prev[j-1]) (horizontal sums
 *            nondecreasing) - for all j=1..6, min(curr[j],prev[j])...
 * Range:     u8 none, u16 n <= 9, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* --- precompute transition matrix once --------------------------------- */
    static unsigned char T[128][128];
    static bool T_ready = false;
    if (!T_ready) {
        for (int p = 0; p < 128; ++p) {
            for (int q = 0; q < 128; ++q) {
                unsigned char ok = 1;
                for (int j = 1; j <= 6; ++j) {
                    int pj   = (p >> j)      & 1;
                    int pjm1 = (p >> (j-1))  & 1;
                    int qj   = (q >> j)      & 1;
                    int qjm1 = (q >> (j-1))  & 1;

                    /* horizontal sums must be nondecreasing */
                    if ((qj + qjm1) < (pj + pjm1)) {
                        ok = 0; break;
                    }
                    /* vertical minima must be nondecreasing */
                    int min_curr_prev   = (qj   < pj)   ? qj  : pj;
                    int min_curr_prev_m1= (qjm1 < pjm1)? qjm1: pjm1;
                    if (min_curr_prev < min_curr_prev_m1) {
                        ok = 0; break;
                    }
                }
                T[p][q] = ok;
            }
        }
        T_ready = true;
    }

    /* --- DP over rows ----------------------------------------------------- */
    seq_t curr[128];
    seq_t next[128];
    for (int i = 0; i < 128; ++i) curr[i] = 1;          /* any first row is allowed */

    for (idx_t step = 0; step < n; ++step) {
        for (int j = 0; j < 128; ++j) next[j] = 0;
        for (int i = 0; i < 128; ++i) {
            seq_t vi = curr[i];
            if (vi == 0) continue;
            for (int j = 0; j < 128; ++j) {
                if (T[i][j]) next[j] += vi;
            }
        }
        for (int j = 0; j < 128; ++j) curr[j] = next[j];
    }

    /* --- sum all final states --------------------------------------------- */
    seq_t total = 0;
    for (int i = 0; i < 128; ++i) total += curr[i];
    return total;
}
