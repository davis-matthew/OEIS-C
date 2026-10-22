/*
 * A344403  a(n) = Sum_{d|n} d * floor(n/d^2).
 * https://oeis.org/A344403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * floor(n/d^2)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if ((n % d) == 0) {
            idx_t div = (n / d) / d;          /* floor(n/d^2) */
            sum += (seq_t)d * (seq_t)div;     /* d * floor(n/d^2) */
        }
    }
    return sum;
}
