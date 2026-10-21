/*
 * A243135  a(n) = 256*n^9 - 576*n^7 + 432*n^5 - 120*n^3 + 9*n.
 * https://oeis.org/A243135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*n^9 - 576*n^7 + 432*n^5 - 120*n^3 + 9*n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 6, u64 n <= 74, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;          // n
    seq_t n2 = x * x;             // n^2
    seq_t n3 = n2 * x;            // n^3
    seq_t n5 = n3 * n2;           // n^5
    seq_t n7 = n5 * n2;           // n^7
    seq_t n9 = n7 * n2;           // n^9

    return (seq_t)256 * n9
         - (seq_t)576 * n7
         + (seq_t)432 * n5
         - (seq_t)120 * n3
         + (seq_t)9   * x;
}
