/*
 * A370104  a(n) = Sum_{k=0..n} (-1)^k * binomial(2*n+k-1,k) *
 *          binomial(6*n-k-1,n-k).
 * https://oeis.org/A370104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n+k-1,k) * *
 *            binomial(4*n-2*k-1,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"   /* for gcd(seq_t, seq_t) */

static inline seq_t binom_multip(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
        res /= den;   /* den always divides res */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    /* iterate k = 0 .. floor(n/2) */
    for (idx_t k = 0; k <= n >> 1; ++k) {
        idx_t a1 = 2 * n + k - 1;
        idx_t b1 = k;
        idx_t a2 = 4 * n - 2 * k - 1;
        idx_t b2 = n - 2 * k;

        seq_t c1 = binom_multip(a1, b1);
        seq_t c2 = binom_multip(a2, b2);
        sum += c1 * c2;
    }
    return sum;
}
