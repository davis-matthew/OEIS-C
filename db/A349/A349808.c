/*
 * A349808  Number of cells in a regular 7-gon after n generations of mitosis.
 * https://oeis.org/A349808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=35*n^2+35*n-20 for n>=1
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t ns = (seq_t)n;
    seq_t term1 = ((seq_t)35) * ns * ns;
    seq_t term2 = ((seq_t)35) * ns;
    seq_t res = term1 + term2 - ((seq_t)20);
    return res;
}
