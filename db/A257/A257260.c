/*
 * A257260  One-based position of the rightmost zero in the factorial base
 *          representation of n (A007623), 0 if no nonleading zeros present.
 * https://oeis.org/A257260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the rightmost zero in the factorial base
 *            representation of n (from the right), 0 if none
 * Range:     u8 n <= 8312, u16 n <= 8312, u32 n <= 8312, u64 n <= 8312,
 *            u128 n <= 8312, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    for (idx_t i = 1; tmp > 0; ++i) {
        idx_t r = tmp % (i + 1);
        if (r == 0) {
            return (seq_t)i;
        }
        tmp /= (i + 1);
    }
    return 0;
}
