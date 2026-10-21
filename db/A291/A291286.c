/*
 * A291286  a(0)=1, a(1)=2, thereafter a(n) = n*a(n-1)+(n-1)*(n-2)*a(n-2).
 * https://oeis.org/A291286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1)+(n-1)*(n-2)*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a_im2 = (seq_t)1;   /* a(0) */
    seq_t a_im1 = (seq_t)2;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = (seq_t)i * a_im1;                        /* n * a(n-1) */
        seq_t term2 = (seq_t)(i-1) * (seq_t)(i-2) * a_im2;     /* (n-1)(n-2) * a(n-2) */
        seq_t ai = term1 + term2;                              /* a(n) */
        a_im2 = a_im1;
        a_im1 = ai;
    }
    return a_im1; /* a(n) */
}
