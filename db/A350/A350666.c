/*
 * A350666  Numbers congruent to 0, 5, and 7 modulo 9: positions of 0 in
 *          A159955.
 * https://oeis.org/A350666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k)=9k, a(3k+1)=5+9k, a(3k+2)=7+9k
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) {
        seq_t k = (seq_t)(n / 3);
        return (seq_t)9 * k;
    } else if (r == 1) {
        seq_t k = (seq_t)((n - 1) / 3);
        return (seq_t)5 + (seq_t)9 * k;
    } else { /* r == 2 */
        seq_t k = (seq_t)((n - 2) / 3);
        return (seq_t)7 + (seq_t)9 * k;
    }
}
