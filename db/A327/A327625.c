/*
 * A327625  Expansion of Sum_{k>=0} x^(3^k) / (1 - x^(3^k))^2.
 * https://oeis.org/A327625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (1/2) * Sum_{d|n} phi(3*d) (multiplicative)
 * Range:     u8 n <= 179, u16 n <= 8567, u32 n <= 8567, u64 n <= 8567,
 *            u128 n <= 8567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative with a(3^e) = (3^(e+1)-1)/2 and a(p^e) = p^e for p != 3 */
/* Initial values: a(1) = 1, a(2) = 2, a(3) = 4, ... */

#include <stdint.h>

/* Exponentiation by squaring for seq_t */
static seq_t pow_u64(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, safe return */

    seq_t res = 1;
    idx_t temp = n;

    /* factor 2 separately */
    int exp = 0;
    while ((temp & 1ULL) == 0)
    {
        exp++;
        temp >>= 1;
    }
    if (exp > 0)
    {
        seq_t val = pow_u64((seq_t)2, exp);
        res *= val;
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2)
    {
        if (temp % p == 0)
        {
            exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
            }
            seq_t val;
            if (p == 3)
                val = (pow_u64((seq_t)3, exp + 1) - 1) / 2;
            else
                val = pow_u64((seq_t)p, exp);
            res *= val;
        }
    }

    /* remaining prime factor > sqrt original n */
    if (temp > 1)
    {
        seq_t val = (temp == 3) ? ((pow_u64((seq_t)3, 2) - 1) / 2) : (seq_t)temp;
        res *= val;
    }

    return res;
}
