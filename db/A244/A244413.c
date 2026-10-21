/*
 * A244413  Exponent of highest power of 8 dividing n.
 * https://oeis.org/A244413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v_8(n) = floor(v_2(n)/3)
 * Range:     u8 n <= 9911, u16 n <= 9911, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* safety for n=0 */
    seq_t count = 0;
    seq_t m = (seq_t)n;
    while (m % 8 == 0) {
        ++count;
        m /= 8;
    }
    return count;
}
