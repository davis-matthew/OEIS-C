/*
 * A286722  Column k=2 of the triangle A225470; Sheffer ((1 - 3*x)^(-2/3),
 *          (-1/3)*log(1 - 3*x)).
 * https://oeis.org/A286722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the elementary symmetric sum of degree n of the *
 *            arithmetic progression 2,5,8,…,2+3(n+1). This can be computed *
 *            iteratively by updating the 0th, 1st, and 2nd coefficients of *
 *            ∏_{j=0}^{n+1} (x + (2+3j)). After all terms are processed, the *
 *            coefficient of x² is the desired value a(n). T...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w0 = 1, w1 = 0, w2 = 0;           /* coefficients of x^0, x^1, x^2 */
    idx_t limit = n + 1;                     /* number of terms = n+2 */
    for (idx_t j = 0; j <= limit; ++j) {
        seq_t val = (seq_t)2 + (seq_t)3 * j; /* 2 + 3j */
        seq_t t2 = w2 * val + w1;            /* new x² coefficient */
        seq_t t1 = w1 * val + w0;            /* new x¹ coefficient */
        seq_t t0 = w0 * val;                 /* new x⁰ coefficient */
        w2 = t2; w1 = t1; w0 = t0;
    }
    return w2;
}
