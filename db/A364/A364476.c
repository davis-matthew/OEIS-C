/*
 * A364476  G.f. satisfies A(x) = 1 + x*A(x) + x^2*A(x)^7.
 * https://oeis.org/A364476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n+5k,k) * binomial(n+4k,n-2k) /
 *            (6k+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Multiplicative binomial with gcd cancellation – works for any seq_t. */
static seq_t binom_custom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = N - K + i;      /* (N-K+i) */
        seq_t den = i;               /* i */

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;                    /* den should now be 1 */

        res = res * num / den;       /* exact division */
    }
    return res;
}

/* Multiply two seq_t and divide by a small denominator using gcd reduction. */
static seq_t mul_div(seq_t a, seq_t b, idx_t denom)
{
    seq_t d = (seq_t)denom;

    seq_t g = gcd(a, d);
    a /= g;
    d /= g;

    g = gcd(b, d);
    b /= g;
    d /= g;

    /* further reduce with the remaining denominator */
    g = gcd(a, d);
    a /= g;
    d /= g;

    g = gcd(b, d);
    b /= g;
    d /= g;

    /* after all cancellations d should be 1 */
    seq_t res = a * b;
    if (d != 1) res /= d;
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k)
    {
        seq_t term1 = binom_custom(n + 5 * k, k);               /* binomial(n+5k,k) */
        seq_t term2 = binom_custom(n + 4 * k, n - 2 * k);       /* binomial(n+4k,n-2k) */
        seq_t term  = mul_div(term1, term2, 6 * k + 1);          /* divide by (6k+1) */
        sum += term;
    }
    return sum;
}
