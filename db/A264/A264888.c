/*
 * A264888  a(n) = n*(n + 1)*(13*n^2 + 13*n - 14)/24.
 * https://oeis.org/A264888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(13*n^2 + 13*n - 14)/24
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 requires special handling because the formula yields a negative
       intermediate value for n = 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t N = n;                                  /* cast index to seq_t */
    seq_t t = N * (N + 1);                         /* n*(n+1)                          */
    seq_t u = (seq_t)13 * N * N +                /* 13*n^2                           */
              (seq_t)13 * N -                /* 13*n                             */
              (seq_t)14;                     /* -14                              */
    seq_t res = t * u;                            /* multiply first (fits in seq_t)   */
    res /= (seq_t)24;                             /* divide by 24 (exact division)    */
    return res;
}
