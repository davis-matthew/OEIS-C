/*
 * A350952  The smallest number whose binary expansion has exactly n distinct
 *          runs.
 * https://oeis.org/A350952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer whose binary representation has
 *            exactly n runs. Construction: start with a 1, then floor(n/2)
 *            zeros, then alternate runs of increasing 1s and decreasing 0s
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t val = 0;
    idx_t zeros_start = n / 2;              /* number of 0‑runs that will appear */

    for (idx_t run = 1; run <= n; ++run) {
        if (run & 1) {                     /* run of 1's */
            idx_t len = (run >> 1) + 1;    /* len = floor(run/2)+1 */
            for (idx_t i = 0; i < len; ++i) {
                val <<= 1;
                val |= 1;
            }
        } else {                           /* run of 0's */
            idx_t len = zeros_start - (run >> 1) + 1;
            for (idx_t i = 0; i < len; ++i) {
                val <<= 1;                 /* OR 0 simply by shifting */
            }
        }
    }
    return val;
}
