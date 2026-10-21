/*
 * A275615  Decimal expansion of 22/111.
 * https://oeis.org/A275615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = cycle [1,9,8] repeating with a(0)=1
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 3) {
        case 0: return (seq_t)1;   /* a(0), a(3), a(6), ... */
        case 1: return (seq_t)9;   /* a(1), a(4), a(7), ... */
        default: return (seq_t)8;  /* a(2), a(5), a(8), ... */
    }
}
