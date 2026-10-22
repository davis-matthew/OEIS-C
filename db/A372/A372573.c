/*
 * A372573  a(n) = 1 if A276085(n) is a multiple of 3, otherwise 0, where
 *          A276085 is the primorial base log-function.
 * https://oeis.org/A372573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 iff exponent of 2 in n and exponent of 3 in n are
 *            congruent mod 3
 * Range:     u8 n <= 9671, u16 n <= 9671, u32 n <= 9671, u64 n <= 9671,
 *            u128 n <= 9671, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    unsigned int e2 = 0;
    while ((t & 1) == 0 && t != 0) {
        e2++;
        t >>= 1;
    }
    t = n;
    unsigned int e3 = 0;
    while (t % 3 == 0 && t != 0) {
        e3++;
        t /= 3;
    }
    return (seq_t)((e2 % 3) == (e3 % 3));
}
