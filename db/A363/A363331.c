/*
 * A363331  a(n) is the sum of divisors of n that are both coreful and
 *          infinitary.
 * https://oeis.org/A363331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of coreful infinitary divisors of n
 * Range:     u8 n <= 16, u16 n <= 2472, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include <stdint.h>

/* Return a(p^e) for a prime power.  */
/*   e is written in binary: e = sum_{k>=0} b(k) * 2^k.
 *   For each bit b(k)=1 we multiply the term
 *     (p^(2^(k+1)) - 1)/(p^(2^k) - 1)
 *   and finally subtract 1.  */
static seq_t prime_power_a(uint64_t p, uint64_t e)
{
    seq_t prod = (seq_t)1;
    uint64_t bit = 1;
    uint64_t k   = 0;

    while (bit <= e)
    {
        if (e & bit)
        {
            /* numerator = p^(2^(k+1)) - 1 */
            seq_t num = seq_pow((seq_t)p, (idx_t)(1ULL << (k + 1))) - (seq_t)1;
            /* denominator = p^(2^k) - 1 */
            seq_t den = seq_pow((seq_t)p, (idx_t)(1ULL << k)) - (seq_t)1;
            prod *= num / den;
        }
        k++;
        bit <<= 1;
    }
    prod -= (seq_t)1;
    return prod;
}

seq_t a(idx_t n)
{
    seq_t ans = (seq_t)1;

    /* factor 2 */
    if ((n & 1ULL) == 0)
    {
        idx_t e = 0;
        idx_t m = n;
        while ((m & 1ULL) == 0)
        {
            m >>= 1;
            e++;
        }
        ans *= prime_power_a(2, e);
        n = m;   /* remaining part */
    }

    /* odd prime factors */
    for (idx_t d = 3; d <= n / d; d += 2)
    {
        if (n % d == 0)
        {
            idx_t e = 0;
            while (n % d == 0)
            {
                n /= d;
                e++;
            }
            ans *= prime_power_a((uint64_t)d, e);
        }
    }

    /* remaining prime larger than sqrt(original n) */
    if (n > 1)
        ans *= prime_power_a((uint64_t)n, 1);

    return ans;
}
