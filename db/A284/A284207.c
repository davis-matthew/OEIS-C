/*
 * A284207  Eleventh column of Euler's difference table in A068106.
 * https://oeis.org/A284207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-10} (-1)^j * binomial(n-10, j) * (n-j)!
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t mul_div(seq_t val, idx_t mul, idx_t denom)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod = (unsigned __int128)val * (unsigned __int128)mul;
    return (seq_t)(prod / denom);
#else
    return (seq_t)((val * mul) / denom);
#endif
}

static inline seq_t factorial(idx_t n)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)0;
    idx_t k = n - 10;
    seq_t term = factorial(n);
    seq_t sum  = term;
    for (idx_t j = 1; j <= k; ++j)
    {
        term = mul_div(term,
                       (idx_t)(k - j + 1),
                       (idx_t)(j * (n - j + 1)));
        if (j & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
