/*
 * A283750  a(n) = n^2 XOR (n + 1)^2.
 * https://oeis.org/A283750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 XOR (n + 1)^2
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 8777, u64 n <= 8777,
 *            u128 n <= 8777, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t sn1 = sn + (seq_t)1;
    seq_t sq1 = sn * sn;
    seq_t sq2 = sn1 * sn1;
    return sq1 ^ sq2;
}
