/*
 * A328994  a(n) = n^2*(1+n)*(1+n^2)/4.
 * https://oeis.org/A328994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)*(n^2+1)/4
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first to avoid intermediate overflows */
    seq_t n_val   = (seq_t)n;           /* n  */
    seq_t n2      = n_val * n_val;      /* n^2 */
    seq_t n1      = n_val + 1;          /* n+1 */
    seq_t n2p1    = n2 + 1;             /* n^2+1 */

    /* We need to divide the product n2 * n1 * n2p1 by 4.
       Divide by 2 twice, always taking the factor that is
       even at each step. This ensures no overflow on any
       of the intermediate values. */
    seq_t a1 = n2;
    seq_t a2 = n1;
    seq_t a3 = n2p1;

    for (int i = 0; i < 2; ++i) {
        if ((a1 & 1) == 0)           /* a1 even? */
            a1 >>= 1;
        else if ((a2 & 1) == 0)       /* a2 even? */
            a2 >>= 1;
        else                          /* a3 must be even */
            a3 >>= 1;
    }

    return a1 * a2 * a3;
}
