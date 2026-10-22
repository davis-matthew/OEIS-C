/*
 * A378425  Expansion of (1/x) * Series_Reversion( x / (1 + x + x^2 * (1 +
 *          x)^3) ).
 * https://oeis.org/A378425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * sum_{k=0..floor(n/2)} C(n+1,k) * C(n+2*k+1,
 *            n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif
#include "../utils/gcd.h"

static seq_t binom_safe(idx_t a, idx_t b)
{
    if (b > a - b) b = a - b;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= b; ++i)
    {
        seq_t num = (seq_t)(a - b + i);
        seq_t den = (seq_t)i;
        seq_t g   = gcd(num, den);
        num /= g;
        den /= g;
        g = gcd(result, den);
        result /= g;
        den /= g;
        result *= num;            /* den should become 1 here */
    }
    return result;
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    using SumType = std::conditional_t<
        std::is_integral<seq_t>::value && std::is_unsigned<seq_t>::value,
        unsigned __int128,
        seq_t>;
#else
    typedef unsigned __int128 SumType;
#endif
    idx_t maxk = n / 2;
    SumType sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t  bin1 = binom_safe(n + 1, k);
        seq_t  bin2 = binom_safe(n + 2 * k + 1, n - 2 * k);
        SumType term = (SumType)bin1 * (SumType)bin2;
        sum += term;
    }
    seq_t result = (seq_t)(sum / ((SumType)n + 1));
    return result;
}
