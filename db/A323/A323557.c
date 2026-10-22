/*
 * A323557  G.f.: Sum_{n>=0} x^n * (1 + x^n)^n / (1 + x^(n+1))^(n+1).
 * https://oeis.org/A323557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * Sum_{m>=0} x^m * (1 + x^m)^m / (1
 *            + x^(m+1))^(m+1). * This equals the sum over all non‑negative
 *            integers n,k,j such that * n + kn + j(n+1) = m of *
 *            binomial(n,k) * binomial(n+j,j) * (-1)^j. * The implementation
 *            evaluates that sum directly. * Complexity: O(n log n) f...
 * Range:     i8 n <= 20, i16 n <= 59, i32 n <= 214, i64 n <= 764,
 *            i128 n <= 2783, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient; works for any seq_t */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n + 1 - i));
        res = res / ((seq_t)i);         /* exact division */
    }
    return res;
}

seq_t a(idx_t m)
{
    seq_t sum = (seq_t)0;

    /* n = 0 contributes the expansion of 1/(1+x) = sum (-1)^j x^j */
    sum = (m & 1ULL) ? -((seq_t)1) : (seq_t)1;

    if (m == 0) return sum;  /* because loops below would be empty */

    /* contributions from n >= 1 */
    for (idx_t n = 1; n <= m; ++n) {
        idx_t maxj = m / (n + 1);    /* j <= floor((m)/(n+1))  */
        for (idx_t j = 0; j <= maxj; ++j) {
            idx_t rem = m - j;
            if (rem % n != 0) continue;          /* n must divide (m-j) */
            idx_t d = rem / n;                    /* d = 1 + k + j   */
            idx_t k = d - 1 - j;                  /* k derived from d */
            if (k > n) continue;                 /* k must be <= n  */
            seq_t t1 = binom_calc(n, k);
            seq_t t2 = binom_calc(n + j, j);
            seq_t term = t1 * t2;
            if (j & 1ULL) term = -term;           /* (-1)^j factor */
            sum += term;
        }
    }

    return sum;
}
