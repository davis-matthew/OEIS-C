/*
 * A382089  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * B(x)^4)
 *          ), where B(x) = 1 + x*B(x)^5 is the g.f. of A002294.
 * https://oeis.org/A382089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{k=0..n-1} (n+1)^(n-k-1) * binomial(4*n + k -
 *            1, k) / (n-k-1)! for n > 0 a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Small helper: fast exponentiation over seq_t. */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Small helper: multiplicative binomial coefficient over seq_t. */
static seq_t binomial_big(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    if (b > a - b) b = a - b;               /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (seq_t)(a - b + i);
        res = res / (seq_t)i;               /* exact division */
    }
    return res;
}

/* Compute OEIS sequence A382089 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                    /* a(0) = 1 */

    seq_t sum = (seq_t)0;
    seq_t fact_ratio = (seq_t)1;            /* (n-1)_k */
    seq_t base = (seq_t)(n + 1);

    for (idx_t k = 0; k < n; ++k) {
        idx_t exp = n - k - 1;              /* exponent for (n+1) */
        seq_t power = pow_uint(base, exp);

        seq_t binom = binomial_big(4 * n + k - 1, k);

        seq_t term = fact_ratio * power * binom;
        sum = sum + term;

        if (k + 1 < n) {
            seq_t factor = (seq_t)(n - k - 1);
            fact_ratio = fact_ratio * factor;
        }
    }
    return sum;
}
