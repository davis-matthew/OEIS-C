/*
 * A348975  a(n) = A003415(n) + A003958(n) - n, where A003415 is the
 *          arithmetic derivative and A003958 is fully multiplicative with
 *          a(p) = (p-1).
 * https://oeis.org/A348975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A003415(n) + A003958(n) - n (Arithmetic derivative +
 *            fully multiplicative function with a(p)=p-1, minus n)
 * Range:     u8 n <= 127, u16 n <= 9512, u32 n <= 9512, u64 n <= 9512,
 *            u128 n <= 9512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_uint64(uint64_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    seq_t num = (seq_t)n;
    seq_t derivative = 0;
    seq_t mult_part  = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            derivative += (num / (seq_t)p) * (seq_t)exp;
            mult_part *= pow_uint64(p - 1, exp);
        }
    }

    if (temp > 1) /* remaining prime factor */
    {
        idx_t p = temp;
        idx_t exp = 1;
        derivative += (num / (seq_t)p) * (seq_t)exp;
        mult_part *= pow_uint64(p - 1, exp);
    }

    return derivative + mult_part - num;
}
