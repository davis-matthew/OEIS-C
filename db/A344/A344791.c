/*
 * A344791  a(n) is the number of 2-point antichains in the poset D_{2n+1} of
 *          type D, whose elements are compositions of 2n+1.
 * https://oeis.org/A344791
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 - (-1)^n - 8*n - 4*n^2 + 8*n^3 + 2*n^4)/32
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 510, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;                 /* n as seq_t */
    seq_t term1 = (n & 1) ? (seq_t)2 : (seq_t)0; /* 1-(-1)^n */

    /* Compute powers of n */
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    seq_t n4 = n2 * n2;

    /* Compute numerator: term1 - 8*n - 4*n^2 + 8*n^3 + 2*n^4 */
    seq_t num = term1;
    num -= tn * (seq_t)8;
    num -= n2 * (seq_t)4;
    num += n3 * (seq_t)8;
    num += n4 * (seq_t)2;

    /* Divide by 32 (which is 2^5). The numerator is always a multiple of 32. */
    return num / (seq_t)32;
}
