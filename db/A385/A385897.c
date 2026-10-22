/*
 * A385897  a(n) = 1 - 5*(n + 1)^2 + 5*(n + 1)^4.
 * https://oeis.org/A385897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - 5*(n + 1)^2 + 5*(n + 1)^4
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n + 1;   // (n + 1)
    seq_t m2 = m * m;          // (n + 1)^2
    seq_t m4 = m2 * m2;        // (n + 1)^4

    seq_t res = 1;
    res -= 5 * m2;             // subtract 5*(n + 1)^2
    res += 5 * m4;             // add 5*(n + 1)^4

    return res;
}
