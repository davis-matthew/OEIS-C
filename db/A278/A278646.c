/*
 * A278646  a(n) = ((2*n+1)/(n+1))*Sum_{j=0..n/2} binomial(n+1,
 *          j)*binomial(n-j-1, n-2*j).
 * https://oeis.org/A278646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n+1)/(n+1))*Σ_{j=0..⌊n/2⌋} C(n+1,j)*C(n-j-1,n-2*j)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplicative binomial coefficient.  Works for 0 ≤ k ≤ n. */
static inline seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;   /* integer division, always exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 (avoids negative arguments in binom). */
    if (n == 0u)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxj = n / 2;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t t1 = binom_multiplicative(n + 1, j);
        seq_t t2 = binom_multiplicative(n - j - 1, n - 2 * j);
        sum += t1 * t2;               /* product fits in seq_t for n where a(n) fits */
    }

    seq_t two_n_plus_one = 2 * (seq_t)n + 1u;
    seq_t denom = (seq_t)(n + 1u);

    seq_t q = sum / denom;        /* quotient part of the division */
    seq_t r = sum % denom;        /* remainder part */

    seq_t part1 = q * two_n_plus_one;
    seq_t part2 = (r * two_n_plus_one) / denom;

    return part1 + part2;
}
