/*
 * A370274  Coefficient of x^n in the expansion of 1/( (1-x) * (1-x^3)^2 )^n.
 * https://oeis.org/A370274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(2*n+k-1,k) *
 *            binomial(2*n-3*k-1,n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binom(2*n + k - 1, k);
        seq_t t2 = binom(2*n - 3*k - 1, n - 3*k);
        sum += t1 * t2;
    }
    return sum;
}
