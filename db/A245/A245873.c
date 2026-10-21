/*
 * A245873  Number of length 4+2 0..n arrays with some pair in every
 *          consecutive three terms totalling exactly n.
 * https://oeis.org/A245873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 7n + 20n² + 16n³ + n⁴ (n even) a(n) = -8 - 3n + 20n²
 *            + 16n³ + n⁴ (n odd) Sequence offset: 1 (a(1) is the first listed
 *            term)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n² */
    seq_t n3 = n2 * nn;          /* n³ */
    seq_t n4 = n3 * nn;          /* n⁴ */

    if ((n & 1) == 0) {          /* even n */
        return (seq_t)1
             + (seq_t)7 * nn
             + (seq_t)20 * n2
             + (seq_t)16 * n3
             + n4;
    } else {                     /* odd n */
        seq_t res = n4 + (seq_t)16 * n3 + (seq_t)20 * n2;
        res -= (seq_t)3 * nn;
        res -= (seq_t)8;
        return res;
    }
}
