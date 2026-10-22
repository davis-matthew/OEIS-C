/*
 * A302710  a(n) = trinomial(2*n, 4) = (1/6)*n*(2*n - 1)*(2*n^2 + 7*n - 3).
 * https://oeis.org/A302710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*n*(2*n - 1)*(2*n^2 + 7*n - 3)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t n_val     = (seq_t)n;
    seq_t two_n     = (seq_t)2 * n_val;                 /* 2*n  */
    seq_t t1        = n_val * (two_n - (seq_t)1);      /* n*(2*n-1) */
    seq_t t2        = (seq_t)2 * n_val * n_val          /* 2*n^2   */
                     + (seq_t)7 * n_val                /* +7*n    */
                     - (seq_t)3;                       /* -3      */
    seq_t num       = t1 * t2;                          /* numerator */

    return num / (seq_t)6;                             /* division by 6 */
}
