/*
 * A375178  a(n) = Sum_{k = 0..n-1} binomial(n+k-1, k)^3 (same as A112028 with
 *          an extra 0 at the start).
 * https://oeis.org/A375178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n-1} binomial(n + k - 1, k)^3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t sum = 0;
    seq_t C = 1;                     /* C = binomial(n + k - 1, k) for current k */

    for (idx_t k = 0; k < n; ++k) {
        /* term = C^3 */
        seq_t term = C * C * C;
        sum += term;

        /* Prepare C for next k: C_{k+1} = C_k * (n + k) / (k + 1) */
        if (k + 1 < n) {
            C = C * (seq_t)(n + k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
