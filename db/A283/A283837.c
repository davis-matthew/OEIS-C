/*
 * A283837  Number of length-n binary vectors beginning with 0, ending with 1,
 *          and avoiding 7 consecutive 0's and 7 consecutive 1's.
 * https://oeis.org/A283837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; For n>=1 count length‑n binary strings that - start with
 *            0, end with 1, - never contain 7 equal consecutive bits.
 *            Recurrence uses a 2×6 state machine (bit, run length). States:
 *            (last_bit ∈ {0,1}, run_len ∈ {1..6}). Transition: if run_len <
 *            6, stay on same bit and inc run_len; always can f...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* special case in OEIS       */
    if (n == 1) return (seq_t)0;          /* only 0, cannot end with 1  */

    /* DP arrays: cur[bit][run], next[bit][run] */
    seq_t cur[2][7] = {{0}};
    seq_t next[2][7] = {{0}};

    /* start with bit 0, run length 1 */
    cur[0][1] = (seq_t)1;

    for (idx_t i = 1; i < n; ++i) {
        /* zero next state */
        for (int b = 0; b < 2; ++b)
            for (int r = 1; r < 7; ++r)
                next[b][r] = (seq_t)0;

        /* transition */
        for (int b = 0; b < 2; ++b) {
            for (int r = 1; r < 7; ++r) {
                seq_t val = cur[b][r];
                if (val == (seq_t)0) continue;     /* skip zero counts   */

                if (r < 6)                           /* stay on same bit   */
                    next[b][r + 1] += val;
                next[1 - b][1] += val;              /* flip bit           */
            }
        }

        /* swap cur and next */
        for (int b = 0; b < 2; ++b)
            for (int r = 1; r < 7; ++r)
                cur[b][r] = next[b][r];
    }

    /* sum over states ending with bit 1 */
    seq_t result = (seq_t)0;
    for (int r = 1; r < 7; ++r)
        result += cur[1][r];

    return result;
}
