/*
 * A365880  Expansion of (1/x) * Series_Reversion( x*(1+x)^4*(1-x)^5 ).
 * https://oeis.org/A365880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/2⌋} binomial(4*n+k+3,k) *
 *            binomial(2*n-2*k,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for fixed-size integer types, if needed */

/* Compute binomial coefficient C(n,k) as a seq_t.
   The algorithm multiplies terms and divides by the
   corresponding denominator at each step, so the
   division is exact.  It works for any seq_t that
   supports +, -, *, / and the usual integer constants. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first, then divide; the result stays exact. */
        seq_t num = n - k + i;
        res = res * num / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 1;

    idx_t half = n / 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= half; ++k) {
        seq_t term1 = binom_calc(4 * n + k + 3, k);
        seq_t term2 = binom_calc(2 * n - 2 * k, n - 2 * k);
        sum += term1 * term2;
    }

    return sum / ((seq_t)(n + 1));
}
