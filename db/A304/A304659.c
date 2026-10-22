/*
 * A304659  a(n) = n*(n + 1)*(16*n - 1)/6.
 * https://oeis.org/A304659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(16*n - 1)/6
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 669, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t val = nt * (nt + 1);
    val = val * (16 * nt - (seq_t)1);
    val = val / (seq_t)6;
    return val;
}
