/*
 * A272100  Integers n that are the sum of three nonzero squares while n*(n+1)
 *          is not.
 * https://oeis.org/A272100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32*n-17-9*(-1)^n)/2 Equivalent: a(n) = 16n-13 for n
 *            even, a(n) = 16n-4 for n odd
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset, safe guard */
    seq_t sixteen_n = ((seq_t)n) << 4;   /* 16 * n */
    if ((n & 1) == 0)              /* n even */
        return sixteen_n - 13;
    else                           /* n odd */
        return sixteen_n - 4;
}
