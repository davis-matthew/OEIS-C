/*
 * A373011  Number of congruences of the 0-twisted Temperley-Lieb monoid of
 *          degree n.
 * https://oeis.org/A373011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)/2 if n is odd; * a(n) = (n+10)/2 if n is even and n
 *            >= 4; * a(0) = 2; * a(2) = 3;
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }
    if (n == 2) {
        return (seq_t)3;
    }
    if (n & 1) {            /* n odd */
        return ((seq_t)n + (seq_t)3) / (seq_t)2;
    } else {                /* n even and n >= 4 */
        return ((seq_t)n + (seq_t)10) / (seq_t)2;
    }
}
