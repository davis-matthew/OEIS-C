/*
 * A288486  Square rings obtained by adding four identical cuboids from
 *          A169938, a(n) = 4*n*(n+1)*(n*(n+1)+1).
 * https://oeis.org/A288486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n+1)*(n*(n+1)+1)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t k  = nn * (nn + 1);
    return (seq_t)4 * k * (k + 1);
}
