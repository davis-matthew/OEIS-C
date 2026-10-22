/*
 * A390255  Expansion of e.g.f. x*(1 + x/2)*sinh(x).
 * https://oeis.org/A390255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n, n even; a(n) = binomial(n,2), n odd
 * Range:     u8 n <= 24, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    if ((n & 1u) == 0u) {          /* even */
        return tn;
    } else {                       /* odd */
        return (tn * (tn - 1u)) / 2u;
    }
}
