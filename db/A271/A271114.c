/*
 * A271114  Expansion of (1+x)*(2+x)/(1-x)^2.
 * https://oeis.org/A271114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2; for n>0, a(n) = 6*n + 1
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    return ((seq_t)n * (seq_t)6) + (seq_t)1;
}
