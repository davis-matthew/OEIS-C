/*
 * A360824  Expansion of Sum_{k>0} (k * x)^k / (1 - k * x^k)^(k+1).
 * https://oeis.org/A360824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d + n/d - 1) * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;
        idx_t m  = n / d;
        idx_t s  = d + m - 1;

        seq_t pow_d = big_pow((seq_t)d, (seq_t)s);
        seq_t bin_d = binomial(s, d);
        result += seq_mul(pow_d, bin_d);

        if (d != m)
        {
            seq_t pow_m = big_pow((seq_t)m, (seq_t)s);
            seq_t bin_m = binomial(s, m);
            result += seq_mul(pow_m, bin_m);
        }
    }
    return result;
}
