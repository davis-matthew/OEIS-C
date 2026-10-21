/*
 * A276916  Subsequence of centered square numbers obtained by adding four
 *          triangles from A276914 and a central element, a(n) = 4*A276914(n)
 *          + 1.
 * https://oeis.org/A276916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(2*n + (-1)^n) + 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t two_n = sn << 1;        /* 2*n */
    seq_t prod;

    if ((n & 1u) == 0)            /* n even: use 2*n + 1 */
        prod = sn * (two_n + 1);
    else                          /* n odd:  use 2*n - 1 */
        prod = sn * (two_n - 1);

    return (prod << 2) + 1;       /* 4*prod + 1 */
}
