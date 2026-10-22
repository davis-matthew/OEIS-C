/*
 * A382043  E.g.f. A(x) satisfies A(x) = 1 + x*A(x)^3*exp(2*x*A(x)).
 * https://oeis.org/A382043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (2*k)^(n-k) * binomial(n+2*k+1,k) /
 *            ((n+2*k+1) * (n-k)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation for seq_t: base^exp */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Sequence A382043 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = 0;
    seq_t F = 1;        /* F_k = n!/(n-k)!   (k=0 -> F=1) */
    seq_t D = 1;        /* D_k = binomial(n+2*k+1,k)   (k=0 -> D=1) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = F * (2*k)^(n-k) * D / (n+2*k+1) */
        seq_t pow_term = seq_pow_uint((seq_t)(2*k), n - k);
        seq_t denom    = (seq_t)(n + 2*k + 1);
        seq_t term     = F;
        term *= pow_term;
        term *= D;
        term /= denom;

        result += term;

        if (k == n) break;           /* no next term */

        /* Prepare F_{k+1} : multiply by (n - k) */
        F *= (seq_t)(n - k);

        /* Update D to D_{k+1} :
           D_{k+1} = D_k * (n+2(k+1))*(n+2(k+1)+1) / ((k+1)*(n+(k+1)+1)) */
        idx_t kp1 = k + 1;
        seq_t num = D;
        num *= (seq_t)(n + 2*kp1);
        num *= (seq_t)(n + 2*kp1 + 1);
        seq_t denomD = (seq_t)kp1 * (seq_t)(n + kp1 + 1);
        D = num / denomD;
    }
    return result;
}
