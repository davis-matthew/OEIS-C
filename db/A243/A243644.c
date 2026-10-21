/*
 * A243644  Expansion of
 *          x*log'(((1-sqrt(1-4*x))/2-sqrt(((-sqrt(1-4*x)-11)*(1-sqrt(1-4*x)))/4+1)+1)/4).
 * https://oeis.org/A243644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n + k - 1, k) *
 *            binomial(3*n - 1, n - 4*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t my_binomial(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - i + 1);
        seq_t denom = (seq_t)i;

        seq_t g = gcd(num, denom);
        num /= g;
        denom /= g;

        seq_t g2 = gcd(result, denom);
        result /= g2;
        denom /= g2;

        /* denom should now be 1 */
        result *= num;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t maxk = n / 4;
    seq_t sum = (seq_t)0;

    for (idx_t k = (idx_t)0; k <= maxk; ++k)
    {
        seq_t c1 = my_binomial(n + k - 1, k);
        seq_t c2 = my_binomial(3 * n - 1, n - 4 * k);
        sum += c1 * c2;
    }
    return sum;
}
