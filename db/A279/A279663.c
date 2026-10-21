/*
 * A279663  a(n) = (5/6)^n*Gamma(n+3/5)*Gamma(n+1)*Gamma(n+2)/Gamma(3/5).
 * https://oeis.org/A279663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} k*(k+1)*(5*k-2)/6
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t a1 = k;
        idx_t b1 = k + 1;
        idx_t c1 = 5 * k - 2;

        /* cancel factor 2 first */
        if ((a1 & 1ULL) == 0) {
            a1 >>= 1;
        } else if ((b1 & 1ULL) == 0) {
            b1 >>= 1;
        } else {
            c1 >>= 1;
        }

        /* cancel factor 3 */
        if (a1 % 3ULL == 0) {
            a1 /= 3;
        } else if (b1 % 3ULL == 0) {
            b1 /= 3;
        } else {
            c1 /= 3;
        }

        seq_t term = (seq_t)a1 * (seq_t)b1 * (seq_t)c1;
        result *= term;
    }
    return result;
}
