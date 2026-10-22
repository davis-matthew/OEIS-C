/*
 * A390682  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(4*n-2*k-3,n-2*k).
 * https://oeis.org/A390682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(4*n-2*k-3, n-2*k)
 *            (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial coefficient C(n,k) without intermediate overflows
   by reducing each step with gcd.  Works for all seq_t types. */
static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num   = n - k + i;
        seq_t denom = i;

        seq_t g = gcd(num, denom);
        if (g != 0) { num /= g; denom /= g; }

        g = gcd(res, denom);
        if (g != 0) { res /= g; denom /= g; }

        res = res * num;
        if (denom != 1) res /= denom;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t max_k = n / 2;
    seq_t pos = 0, neg = 0;

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t r = n - 2 * k;               /* n-2*k */
        idx_t N = 4 * n - 3 - 2 * k;       /* 4*n-2*k-3 */
        seq_t term = binom_multiplicative(N, r);

        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;   /* pos >= neg for all n */
}
