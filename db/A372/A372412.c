/*
 * A372412  Coefficient of x^n in the expansion of ( (1-x+x^2) / (1-x)
 *          )^(3*n).
 * https://oeis.org/A372412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n, k) * binomial(n-k-1,
 *            n-2*k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for unsigned indices using only seq_t arithmetic.
   Constant-time division is safe because the intermediate results are always divisible. */
static seq_t binom_helper(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;           /* use symmetry to minimize the loop */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by the next numerator term */
        result = result * ((seq_t)(n - k + i));
        /* divide by the denominator term */
        result = result / ((seq_t)i);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;        /* a(0) = 1 by definition */
    idx_t maxk = n / 2;
    seq_t total = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binom_helper(3 * n, k);               /* binomial(3*n, k) */
        seq_t term2 = binom_helper(n - k - 1, n - 2 * k);   /* binomial(n-k-1, n-2k) */
        total = total + term1 * term2;
    }
    return total;
}
