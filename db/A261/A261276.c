/*
 * A261276  100-gonal numbers: a(n) = 98*n*(n-1)/2 + n.
 * https://oeis.org/A261276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(49*n - 48)
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 9362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return m * (m * 49 - 48);
}
