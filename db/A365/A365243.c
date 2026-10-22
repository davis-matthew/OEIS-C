/*
 * A365243  G.f. satisfies A(x) = 1 + x*A(x)/(1 - x^3*A(x)^2).
 * https://oeis.org/A365243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n-2*k-1, k) * binomial(n-k+1,
 *            n-3*k) / (n-k+1)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A365243, offset 0) */
#include "../utils/gcd.h"

/* multiplicative binomial coefficient using gcd-based cancellation */
static seq_t my_binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;                 /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t denom = (seq_t)i;
        seq_t g = gcd(res, denom);
        res /= g;
        denom /= g;
        res = res * num;
        res = res / denom;
    }
    return res;
}

/* sequence A365243 */
seq_t a(idx_t n)
{
    if (n <= 3U) return (seq_t)1;

    seq_t sum = 0;
    idx_t kmax = n / 3U;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t r1 = my_binomial(n - 2U * k - 1U, k);
        seq_t r2 = my_binomial(n - k + 1U, n - 3U * k);
        seq_t denom = (seq_t)(n - k + 1U);

        seq_t g = gcd(r1, denom);
        r1 /= g;
        denom /= g;
        g = gcd(r2, denom);
        r2 /= g;
        denom /= g; /* denom should be 1 now */

        seq_t term = r1 * r2;
        sum += term;
    }
    return sum;
}
