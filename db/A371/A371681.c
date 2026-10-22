/*
 * A371681  G.f. satisfies A(x) = ( 1 + x * A(x) * (1 + A(x)) )^3.
 * https://oeis.org/A371681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371681
   a(n) = Σ_{k=0..n} binomial(n,k) * binomial(3*n+3*k+3,n) / (n+k+1)   */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;                 /* running total */
    seq_t binom_nk = 1;             /* C(n,0) */

    /* C(3n+3, n) for k = 0 */
    seq_t binom2 = 1;
    seq_t base = (seq_t)(3*n + 3);  /* 3*n + 3 */
    for (idx_t i = 1; i <= n; ++i) {
        /* (base - n + i) = 2*n + 3 + i */
        seq_t numerator = base - n + i;
        binom2 = binom2 * numerator / i;
    }

    for (idx_t k = 0; k <= n; ++k) {
        /* current term */
        seq_t term = binom_nk * binom2;
        term = term / (seq_t)(n + k + 1);
        sum += term;

        if (k == n) break;          /* finished all k */

        /* update C(n,k+1) from C(n,k) */
        binom_nk = binom_nk * (n - k) / (k + 1);

        /* update C(3n+3k+6, n) from C(3n+3k+3, n) */
        seq_t N = (seq_t)(3*n + 3*k + 3);   /* current N = 3n + 3k + 3 */

        /* multiply by (N+1)/(N+1-n) */
        seq_t num = N + 1;
        seq_t den = N + 1 - n;
        binom2 = binom2 * num / den;

        /* multiply by (N+2)/(N+2-n) */
        num = N + 2;
        den = N + 2 - n;
        binom2 = binom2 * num / den;

        /* multiply by (N+3)/(N+3-n) */
        num = N + 3;
        den = N + 3 - n;
        binom2 = binom2 * num / den;
    }

    return sum;
}
