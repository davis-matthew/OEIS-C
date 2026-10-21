/*
 * A273220  a(n) = 8n^2 - 12n + 1.
 * https://oeis.org/A273220
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 12*n + 1
 * Range:     u8 n <= 6, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 8 * tn * tn - 12 * tn + 1;
}
