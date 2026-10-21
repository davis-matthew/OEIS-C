/*
 * A263120  Number of (n+3)X(2+3) 0..1 arrays with each row divisible by 15
 *          and column not divisible by 15, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A263120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of (n+3)×5 binary arrays where every row
 *            * (read MSB‑left) is a multiple of 15 and every column (read
 *            MSB‑top) * is NOT a multiple of 15. This is computed by dynamic
 *            programming * over the partial column sums modulo 15. The state
 *            is the triple * (s0,d1,d4): * s0 = column 0...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Number of rows */
    idx_t L = n + (idx_t)3;
    if (L == 0) return 0;         /* safety, though n≥1 */

    /* Constant size of state space */
    const int STATE = 15 * 15 * 15;   /* 3375 */
    const int STEP  = 15 * 15;        /* 225 */

    /* DP arrays */
    seq_t dp[STATE];
    seq_t ndp[STATE];
    for (int i = 0; i < STATE; ++i) dp[i] = 0;
    dp[0] = 1;                 /* (s0,d1,d4) = (0,0,0) */

    /* Precompute component values for each state to avoid divisions */
    int s0_arr[STATE];
    int d1_arr[STATE];
    int d4_arr[STATE];
    for (int idx = 0; idx < STATE; ++idx) {
        s0_arr[idx] = idx / STEP;
        int rem = idx % STEP;
        d1_arr[idx] = rem / 15;
        d4_arr[idx] = rem % 15;
    }

    /* Weight cycle: 1,2,4,8 with period 4.
       For the top row we need exponent L-1, so start index is (L-1) mod 4.
       Each subsequent row moves backwards through the cycle. */
    int cycle_idx = (int)((L - 1) & 3);   /* (L-1) % 4 since 4 is power of two */

    for (idx_t row = 0; row < L; ++row) {
        int w = 1 << cycle_idx;          /* weight for this row (1,2,4,8) */
        /* reset next DP array */
        for (int i = 0; i < STATE; ++i) ndp[i] = 0;

        for (int idx = 0; idx < STATE; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;

            int s0 = s0_arr[idx];
            int d1 = d1_arr[idx];
            int d4 = d4_arr[idx];

            /* Transition 1: R0 (no change) */
            ndp[idx] += val;

            /* Transition 2: R1 */
            int s0p = s0 + w; if (s0p >= 15) s0p -= 15;
            int d4p = d4 + (15 - w); if (d4p >= 15) d4p -= 15;
            int idx1 = s0p * STEP + d1 * 15 + d4p;
            ndp[idx1] += val;

            /* Transition 3: R2 */
            int d1p = d1 + w; if (d1p >= 15) d1p -= 15;
            int idx2 = s0 * STEP + d1p * 15 + d4;
            ndp[idx2] += val;
        }

        /* swap dp and ndp pointers (by copying) */
        for (int i = 0; i < STATE; ++i) dp[i] = ndp[i];

        /* advance to next weight in cycle (decrement mod 4) */
        cycle_idx = (cycle_idx + 3) & 3;   /* same as (cycle_idx-1)%4 */
    }

    /* Sum states where all columns are non‑zero modulo 15 */
    seq_t result = 0;
    for (int idx = 0; idx < STATE; ++idx) {
        seq_t val = dp[idx];
        if (!val) continue;

        int s0 = s0_arr[idx];
        int d1 = d1_arr[idx];
        int d4 = d4_arr[idx];

        if (s0 != 0 &&
            ((s0 + d1) % 15) != 0 &&
            ((s0 + d1 + d4) % 15) != 0) {
            result += val;
        }
    }
    return result;
}
