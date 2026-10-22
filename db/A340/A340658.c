/*
 * A340658  The number of overpartitions of n having more non-overlined parts
 *          than overlined parts.
 * https://oeis.org/A340658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 83, u64 n <= 274,
 *            u128 n <= 966, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A340658 – number of overpartitions of n having more nonoverlined parts
    than overlined parts.  This implementation uses only integer arithmetic
    and works for any seq_t / idx_t type provided by the build environment.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    /* ---------- compute A[n] = coeff. of q^n in ∏ (1+q^k)/(1-q^k) ----------
       Each factor has coefficients 1 at exponent 0 and 2 at all positive
       multiples of k.  */
    seq_t *dpA   = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *newdpA = (seq_t*)calloc(N + 1, sizeof(seq_t));
    dpA[0] = (seq_t)1;

    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t r = 0; r < k; ++r) {
            seq_t cum = 0;
            size_t j = 0;
            for (size_t s = r; s <= N; s += k, ++j) {
                newdpA[s] = dpA[s] + (seq_t)2 * cum;
                cum += dpA[s];
            }
        }
        /* swap dpA and newdpA for next iteration */
        seq_t *tmp = dpA;
        dpA = newdpA;
        newdpA = tmp;
    }

    /* dpA now holds the coefficients A[0..N] */
    seq_t *dpB_cur = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *newdpB = (seq_t*)calloc(N + 1, sizeof(seq_t));
    dpB_cur[0] = (seq_t)1;           /* t = 0   : empty product */

    seq_t *B = (seq_t*)calloc(N + 1, sizeof(seq_t));  /* B[0..N] */

    /* --------- build B[n] = ∑_{t>=0} coeff_{t}[n‑t(t+1)/2] ----------
       coeff_{t}[m] = coefficient of q^m in ∏_{k=1..t} (1−q^k)^{-2}   */
    idx_t tmax = 0;
    while (((tmax + 1) * (tmax + 2)) / 2 <= n) ++tmax;

    /* t = 0 contribution (shift = 0) */
    if (tmax >= 0) {
        B[0] = dpB_cur[0];
    }

    for (idx_t t = 1; t <= tmax; ++t) {
        /* Update dpB_cur by multiplying with (1−q^t)^{-2}
           newdpB[s] = Σ_{m=0}^{⌊s/t⌋} (m+1) * dpB_cur[s−m*t]   */
        for (idx_t r = 0; r < t; ++r) {
            seq_t sumRaw   = 0;
            seq_t sumWeight = 0;
            idx_t j        = 0;
            for (size_t s = r; s <= N; s += t, ++j) {
                seq_t old = dpB_cur[s];
                sumRaw   += old;
                sumWeight += old * (seq_t)j;
                newdpB[s] = (seq_t)(j + 1) * sumRaw - sumWeight;
            }
        }
        /* swap arrays */
        seq_t *tmp = dpB_cur;
        dpB_cur = newdpB;
        newdpB = tmp;

        /* Add contribution for current t */
        idx_t shift = t * (t + 1) / 2;
        if (shift > N) break;
        for (idx_t m = 0; m + shift <= N; ++m) {
            B[m + shift] += dpB_cur[m];
        }
    }

    /* ---------- final result a[n] = A[n] – B[n] ---------- */
    seq_t result = dpA[N] - B[N];

    /* clean up */
    free(dpA);      free(newdpA);
    free(dpB_cur);  free(newdpB);
    free(B);

    return result;
}
