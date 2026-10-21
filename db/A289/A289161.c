/*
 * A289161  Number of 3-cycles in the n X n black bishop graph.
 * https://oeis.org/A289161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^2 * (2*n^2 - 4*n + 3 - 3*(-1)^n) / 24
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 309, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n1 = ns - 1;

    /* Compute the inner factor:
       2*n^2 - 4*n + 3 - 3*(-1)^n  */
    seq_t term = 2 * ns * ns - 4 * ns;
    /* If n is odd (-1)^n = -1, subtracting -3 adds 6 */
    if ((n & 1) == 1)
        term += 6;

    seq_t res = n1 * n1 * term;
    res /= 24;           /* division is exact in all cases */
    return res;
}
