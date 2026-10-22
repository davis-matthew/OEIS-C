/*
 * A304357  Antidiagonal sums of the first quadrant of array A(k,m) = F_k(m),
 *          F_k(m) being the k-th Fibonacci polynomial evaluated at m.
 * https://oeis.org/A304357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} F_j(n-j)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t m = (seq_t)(n - j);
        seq_t f0 = 0;         /* F_0(m) */
        seq_t f1 = 1;         /* F_1(m) */
        seq_t fj;
        if (j == 0) {
            fj = f0;
        } else if (j == 1) {
            fj = f1;
        } else {
            seq_t f2;
            for (idx_t k = 2; k <= j; ++k) {
                f2 = m * f1 + f0;
                f0 = f1;
                f1 = f2;
            }
            fj = f1;
        }
        sum += fj;
    }
    return sum;
}
