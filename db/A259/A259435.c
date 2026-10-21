/*
 * A259435  a(n) = 2*(n-1)^6*(n+1)^2*(n^2+10*n+1).
 * https://oeis.org/A259435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)^6*(n+1)^2*(n^2+10*n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n == 0 (negative base in the formula) */
    if (n == 0)
        return (seq_t)2;

    seq_t n_      = (seq_t)n;
    seq_t n_minus1 = n_ - 1;      /* n >= 1 here, so no underflow */
    seq_t n_plus1  = n_ + 1;

    /* Compute (n-1)^6 */
    seq_t t = n_minus1 * n_minus1;           /* (n-1)^2 */
    t = t * t;                               /* (n-1)^4 */
    t = t * n_minus1;                        /* (n-1)^5 */
    t = t * n_minus1;                        /* (n-1)^6 */

    /* Compute (n+1)^2 */
    seq_t t2 = n_plus1 * n_plus1;

    /* Compute (n^2 + 10*n + 1) */
    seq_t n_sq   = n_ * n_;
    seq_t n10    = n_ * 10;
    seq_t t3 = n_sq + n10 + (seq_t)1;

    /* Combine the factors */
    seq_t res = (seq_t)2;
    res = res * t;
    res = res * t2;
    res = res * t3;

    return res;
}
