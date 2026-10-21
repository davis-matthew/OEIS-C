/*
 * A298028  Coordination sequence of Dual(3.6.3.6) tiling with respect to a
 *          trivalent node.
 * https://oeis.org/A298028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(2*k) = 12*k; a(2*k+1) = 6*k+3
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* floor(n/2) converted to seq_t */
    seq_t k = (seq_t)(n >> 1);
    if ((n & 1) == 0) {          /* even index */
        return k * (seq_t)12;
    } else {                     /* odd index */
        return k * (seq_t)6 + (seq_t)3;
    }
}
