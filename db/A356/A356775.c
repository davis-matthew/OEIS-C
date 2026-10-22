/*
 * A356775  Coefficients in the power series expansion of A(x) =
 *          Sum_{n=-oo..+oo} n*(n+1)/2 * x^(2*n) * (1 - x^n)^(n-2).
 * https://oeis.org/A356775
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 + sum_{d|n, d≥2} [ (for q = n/d) * if q ≥ d term_neg =
 *            (-1)^d * d(d-1)/2 * binom(q+1, d+1) * if 2 ≤ q ≤ d term_pos =
 *            d(d+1)/2 * (-1)^{q-2} * binom(d-2, q-2) * (Sum over all divisors
 *            d of n) * offset: 2
 * Range:     i8 n <= 15, i16 n <= 63, i32 n <= 263, i64 n <= 1231,
 *            i128 n <= 2050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Utility: binomial coefficient using multiplicative formula with __int128 intermediate.
   Works for n,k up to 10000; for larger uses __int128 which may overflow but
   such terms typically exceed the limits of seq_t. */
static seq_t binom_uint64(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (__int128)(n - k + i);
        res = res / (__int128)i;
    }
    return (seq_t)res;
}

/* Contribution of a single divisor d of n. */
static seq_t contrib_for_divisor(idx_t d, idx_t n)
{
    seq_t sum = 0;
    idx_t q = n / d; /* n = d * q, integer division since d | n */

    /* (1) Negative side contribution (present for q ≥ d) */
    if (q >= d) {
        seq_t coeff = (seq_t)d * (d - 1) / 2;       /* d(d-1)/2 */
        if (d & 1) coeff = -coeff;                 /* factor (-1)^d */
        seq_t bin  = binom_uint64((uint64_t)q + 1, (uint64_t)d + 1);
        sum += coeff * bin;
    }

    /* (2) Positive side contribution (present for 2 ≤ q ≤ d) */
    if (q >= 2 && q <= d) {
        uint64_t k = q - 2;                        /* k = q-2 */
        seq_t coeff = (seq_t)d * (d + 1) / 2;       /* d(d+1)/2 */
        if (k & 1) coeff = -coeff;                 /* factor (-1)^{k} = (-1)^{q-2} */
        seq_t bin  = binom_uint64((uint64_t)d - 2, k);
        sum += coeff * bin;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* values below the offset are irrelevant */

    seq_t total = 1; /* contribution from the n = 1 term of the series */

    /* iterate over all divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        idx_t d1 = d;
        if (d1 >= 2) total += contrib_for_divisor(d1, n);

        idx_t d2 = n / d;
        if (d2 != d1 && d2 >= 2) total += contrib_for_divisor(d2, n);
    }

    return total;
}
