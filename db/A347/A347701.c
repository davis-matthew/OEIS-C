/*
 * A347701  Rightmost diagonal of A347700.
 * https://oeis.org/A347701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(n+2)/3 - 2*n for n>=2; a(1)=2
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    seq_t ns = (seq_t)n;
    /* n*(n+1)*(n+2) is divisible by 3 for all integers n */
    seq_t term = ns * (ns + (seq_t)1) * (ns + (seq_t)2) / (seq_t)3
                 - (seq_t)2 * ns;
    return term;
}
