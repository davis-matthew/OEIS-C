/*
 * A369226  Expansion of (1/x) * Series_Reversion( x * (1-x) / (1+x^2)^2 ).
 * https://oeis.org/A369226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/2⌋} binomial(2*n+2,k) *
 *            binomial(2*n-2*k,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply the two binomial coefficients using a safe, multiplicative algorithm. */
static seq_t binomial_mul(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;         /* use symmetry to reduce k */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

/* Sequence A369226 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;              /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m1 = 2 * n + 2;
        idx_t m2 = 2 * n - 2 * k;
        idx_t r1 = n - 2 * k;
        seq_t b1 = binomial_mul(m1, k);
        seq_t b2 = binomial_mul(m2, r1);
        sum += b1 * b2;
    }
    return sum / (seq_t)(n + 1);
}
