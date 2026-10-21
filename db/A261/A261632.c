/*
 * A261632  Expansion of Product_{k>=0} 1/(1-x^(4*k+1))^3.
 * https://oeis.org/A261632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Coefficients of ∏_{k≥0} 1/(1‑x^(4k+1))^3 – the OEIS sequence
 *            A261632
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function a(n) returns the n‑th term of the sequence.
   It uses dynamic programming with the recurrence
   b_t = 3*b_{t-1} – 3*b_{t-2} + b_{t-3} + a_t for each part size m ≡ 1 (mod 4). */
seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;
    dp[0] = 1;                         /* a(0) = 1 */

    for (idx_t m = 1; m <= n; m += 4) {
        seq_t *newdp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        if (!newdp) { free(dp); return 0; }

        /* Process each residue class modulo m separately. */
        for (idx_t r = 0; r < m; ++r) {
            idx_t idx = r;
            seq_t b_prev0 = 0, b_prev1 = 0, b_prev2 = 0;       /* b_{t‑1}, b_{t‑2}, b_{t‑3} */

            for (idx_t t = 0; idx <= n; ++t, idx += m) {
                seq_t a_t = dp[idx];
                seq_t b_t;

                if (t == 0)                     /* w_0 = 1 */
                    b_t = a_t;
                else if (t == 1)                 /* w_1 = 3 */
                    b_t = 3 * dp[idx - m] + a_t;
                else if (t == 2)                 /* w_2 = 6 */
                    b_t = 6 * dp[idx - 2 * m] + 3 * dp[idx - m] + a_t;
                else                              /* recurrence from w_k = 3w_{k-1} – 3w_{k-2} + w_{k-3} */
                    b_t = 3 * b_prev0 - 3 * b_prev1 + b_prev2 + a_t;

                newdp[idx] = b_t;

                /* Shift previous b values for the next iteration. */
                b_prev2 = b_prev1;
                b_prev1 = b_prev0;
                b_prev0 = b_t;
            }
        }

        free(dp);
        dp = newdp;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
