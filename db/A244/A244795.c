/*
 * A244795  Number of length 7 0..n arrays with each partial sum starting from
 *          the beginning no more than one standard deviation from its mean.
 * https://oeis.org/A244795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   alloc_seq_array, floor_sqrt, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A244795
 * Number of length‑7 arrays with elements 0..n such that for each k=1..7
 * the partial sum S_k satisfies |S_k – k·n/2| ≤ √(k·n(n+2)/12)
 *
 * This implementation uses a dynamic programming approach with only
 * integer arithmetic.  At step k the admissible sums are the interval
 * [low_k, high_k] obtained from
 *
 *   (2·S_k – k·n)^2 ≤ k·(n^2+2n)/3
 *
 * which is equivalent to
 *
 *   low_k  = ceil((k·n – M_k)/2)
 *   high_k = floor((k·n + M_k)/2)
 *
 * where M_k = ⌊√(k·(n^2+2n)/3)⌋.  The DP state dp[s] counts ways to
 * reach sum s after the current number of positions; transitions are
 * a sliding‑window convolution of width n+1.  The algorithm runs in
 * O(7·n) time and O(n) memory.  It works for any supported seq_t (8–128‑bit
 * unsigned or an arbitrary‑precision class) and any idx_t (uint64_t). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* defined only for n ≥ 1 */

    /*   0 ≤ S_7 ≤ 7·n   */
    const idx_t maxSum = 7 * n;

    /* Allocate arrays for DP and for prefix sums.  All elements are
       initialized to zero in the allocation helper. */
    seq_t *dpPrev = alloc_seq_array(maxSum + 1);
    seq_t *dpCurr = alloc_seq_array(maxSum + 1);
    seq_t *pref   = alloc_seq_array(maxSum + 1);

    /* Base case: before any element, the only possible sum is 0. */
    for (idx_t i = 1; i <= maxSum; ++i) dpPrev[i] = (seq_t)0;
    dpPrev[0] = (seq_t)1;

    /* Current admissible range for the previous step.  Initially
       only sum 0 is allowed. */
    idx_t prevLow  = 0;
    idx_t prevHigh = 0;

    /* Pre‑compute n as seq_t for later use. */
    seq_t tn = (seq_t)n;

    for (idx_t k = 1; k <= 7; ++k) {
        /* Compute M_k = floor( sqrt( k*(n^2+2n)/3 ) ).  All
           intermediate values fit into seq_t for the intended n. */
        seq_t n_term = tn * tn + (seq_t)2 * tn;          /* n^2 + 2n */
        seq_t bound  = (seq_t)k * n_term;              /* k*(n^2+2n) */
        seq_t bound_div3 = bound / 3;                  /* integer division is safe */
        seq_t M_k = floor_sqrt(bound_div3);
        idx_t M = (idx_t)M_k;                          /* convert to idx_t */

        /* Compute admissible interval for sum after k elements. */
        idx_t kn = k * n;                              /* k·n  (fits in 64 bits) */
        idx_t low_k = (kn > M) ? (kn - M + 1) / 2 : 0;  /* ceil((kn-M)/2) */
        idx_t high_k = (kn + M) / 2;                   /* floor((kn+M)/2) */
        if (high_k > kn) high_k = kn;                 /* safety, though never needed */

        /* If no admissible sums exist, the answer is zero. */
        if (low_k > high_k) {
            free_seq_array(dpPrev);
            free_seq_array(dpCurr);
            free_seq_array(pref);
            return (seq_t)0;
        }

        /* Zero the entire dpCurr array for safety. */
        for (idx_t i = 0; i <= maxSum; ++i) dpCurr[i] = (seq_t)0;

        /* Build prefix sums of dpPrev for fast range sums. */
        pref[0] = dpPrev[0];
        for (idx_t i = 1; i <= maxSum; ++i) {
            pref[i] = pref[i - 1] + dpPrev[i];
        }

        /* Compute dpCurr[s] for all admissible s in [low_k, high_k]. */
        for (idx_t s = low_k; s <= high_k; ++s) {
            /* Previous sum must satisfy both the window constraint
               and being admissible from the previous step. */
            idx_t low_prev = (s >= n) ? s - n : 0;
            if (low_prev < prevLow) low_prev = prevLow;
            idx_t high_prev = (s < prevHigh) ? s : prevHigh;

            if (low_prev > high_prev) {
                dpCurr[s] = (seq_t)0;
                continue;
            }

            seq_t val = pref[high_prev];
            if (low_prev > 0) val -= pref[low_prev - 1];
            dpCurr[s] = val;
        }

        /* Prepare for next iteration: dpPrev <- dpCurr. */
        seq_t *tmp = dpPrev;
        dpPrev = dpCurr;
        dpCurr = tmp;
        prevLow  = low_k;
        prevHigh = high_k;
    }

    /* Sum all counts for the final admissible sums. */
    seq_t ans = (seq_t)0;
    for (idx_t s = prevLow; s <= prevHigh; ++s) ans += dpPrev[s];

    free_seq_array(dpPrev);
    free_seq_array(dpCurr);
    free_seq_array(pref);

    return ans;
}
