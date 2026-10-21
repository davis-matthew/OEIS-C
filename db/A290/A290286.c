/*
 * A290286  Determinant of circulant matrix of order 4 with entries in the
 *          first row (-1)^j*Sum_{k>=0}(-1)^k*binomial(n, 4*k+j), j=0,1,2,3.
 * https://oeis.org/A290286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = determinant of the 4×4 circulant matrix * with first row
 *            r_j = (-1)^j * Σ_{k≥0} (-1)^k * binom(n, 4k+j), j = 0..3. *
 *            Computed by using eigenvalues λ_m = Σ r_j ω_4^{mj} (ω_4 = i) and
 *            * det = λ0 * λ2 * ((r0 - r2)^2 + (r1 - r3)^2)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum[4] = {0, 0, 0, 0};
    seq_t cur = 1;              /* C(n,0) */
    for (idx_t m = 0; m <= n; ++m)
    {
        int j = (int)(m & 3ULL);                /* m mod 4 */
        idx_t k = (m - j) >> 2;                 /* floor((m-j)/4) */
        seq_t sign = (k & 1ULL) ? (seq_t)-1 : (seq_t)1;
        sum[j] += sign * cur;
        if (m == n) break;                     /* last term reached */
        /* cur <- C(n, m+1) = cur * (n-m)/(m+1) */
        cur = cur * (seq_t)(n - m) / (seq_t)(m + 1);
    }

    seq_t r0 =  sum[0];
    seq_t r1 = -sum[1];
    seq_t r2 =  sum[2];
    seq_t r3 = -sum[3];

    seq_t lam0 = r0 + r1 + r2 + r3;
    seq_t lam2 = r0 - r1 + r2 - r3;
    seq_t d0 = r0 - r2;
    seq_t d1 = r1 - r3;
    seq_t modsq = d0 * d0 + d1 * d1;

    return lam0 * lam2 * modsq;
}
