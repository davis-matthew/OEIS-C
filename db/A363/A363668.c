/*
 * A363668  a(n) = Sum_{d|n} (n/d)^d * binomial(d+n-1,d).
 * https://oeis.org/A363668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^d * binomial(d+n-1,d)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow, seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Offset: 1 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_gcd.h"

static inline seq_t _binom(idx_t n, idx_t k)
{
    /* compute binomial(n,k) safely for values that fit in seq_t */
    if (k > n - k) k = n - k;
    if (k == 0) return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* cancel gcd between numerator and denominator */
        seq_t g = seq_gcd(num, den);
        num /= g;
        den /= g;

        /* cancel remaining gcd with current result */
        g = seq_gcd(res, den);
        res /= g;
        den /= g;

        /* product, division by any remaining factor */
        seq_t tmp = seq_mul(res, num);
        res = tmp / den;   /* den should be 1 here */
    }
    return res;
}

static inline seq_t _contrib(idx_t d, idx_t n)
{
    seq_t power = big_pow((seq_t)(n / d), (seq_t)d);
    seq_t binom = _binom(d + n - 1, d);
    return seq_mul(power, binom);
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            sum += _contrib(d, n);
            idx_t e = n / d;
            if (e != d)
                sum += _contrib(e, n);
        }
    }
    return sum;
}
