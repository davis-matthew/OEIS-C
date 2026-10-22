/*
 * A384165  a(n) = Product_{k=0..n-1} (3*n+2*k).
 * https://oeis.org/A384165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0}^{n-1} (3*n + 2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return ((seq_t)1);
    seq_t result = ((seq_t)1);
    seq_t tn = (seq_t)n;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term = ((seq_t)3 * tn) + ((seq_t)2 * (seq_t)k);
        result *= term;
    }
    return result;
}
