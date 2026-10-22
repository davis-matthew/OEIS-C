/*
 * A383132  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n*k,k) * n^k.
 * https://oeis.org/A383132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n*k,k) * n^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for any standard integer types used by the build */

/* Helper: multiplicative binomial coefficient.
   Computes C(n,k) exactly using only integer operations. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;           /* use symmetry to minimise work */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / (seq_t)i;           /* exact division shrinks intermediate */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t power_of_n = 1;              /* n^0 initially */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binom(n, k);
        seq_t c2 = binom(n * k, k);    /* compute C(n*k, k) */
        seq_t term = c1 * c2;
        term = term * power_of_n;      /* multiply by n^k */
        total = total + term;
        power_of_n = power_of_n * (seq_t)n;   /* next power n^(k+1) */
    }
    return total;
}
