/*
 * A319392  a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n,k)*k!*n^k.
 * https://oeis.org/A319392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * k! * n^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t term = 1;          /* term_k = C(n,k) * k! * n^k, starting with k = 0 */
    seq_t pos   = 0;         /* sum of terms with positive sign   */
    seq_t neg   = 0;         /* sum of terms with negative sign   */
    int   sign  = ((n & 1) ? -1 : 1);   /* sign for k = 0 : (-1)^(n) */

    for (idx_t k = 0; k <= n; ++k) {
        if (sign > 0) {
            pos += term;
        } else {
            neg += term;
        }

        if (k == n) break;          /* finished last term */

        /* Update term for next k:
           term_{k+1} = term_k * (n - k) * n  (derived from the factorial/binomial relation) */
        term *= (seq_t)(n - k);
        term *= (seq_t)n;

        sign = -sign;                /* alternate sign for next term */
    }

    return pos - neg;                 /* final result is non‑negative */
}
