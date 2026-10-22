/*
 * A316593  a(n) equals the coefficient of x^n in Sum_{m>=0} (x^m + 3 +
 *          1/x^m)^m for n >= 1.
 * https://oeis.org/A316593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m|n, m>=1} * sum_{k=0}^{floor((m - n/m)/2)} * C(m,
 *            n/m + k) * * C(m - (n/m + k), m - n/m - 2k) * * 3^{m - n/m - 2k}
 *            * * The coefficient of x^n in Σ_{m≥0} (x^m + 3 + x^-m)^m
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* local helper: binomial coefficient,   n,k are idx_t   */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;      /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) then divide by i (exact division) */
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

/* local helper: power of 3   exp <= idx_t   */
static seq_t pow3_u64(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * 3;
    return r;
}

/* local helper: contribution for a fixed pair (m,s) = (m, n/m)   */
static seq_t contrib(idx_t m, idx_t s)
{
    if (s > m) return 0;          /* no solutions when s>m */
    idx_t maxk = (m - s) / 2;      /* floor((m−s)/2)          */
    seq_t total = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t a = s + k;                       /* a = s + k              */
        idx_t b = m - s - 2 * k;               /* b = m − s − 2k          */
        seq_t term = binomial(m, a);
        term = term * binomial(m - a, b);
        if (b > 0)
            term = term * pow3_u64(b);
        total = total + term;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence is defined for n>=1 */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor pair (m=d, s=n/d) */
            sum = sum + contrib(d, n / d);
            /* complementary divisor pair (m=n/d, s=d) if distinct */
            if (d * d != n)
                sum = sum + contrib(n / d, d);
        }
    }
    return sum;
}
