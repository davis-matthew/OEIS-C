/*
 * A385541  Number of divisors of n such that d^d == (-d)^d == d (mod n).
 * https://oeis.org/A385541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d | d divides n : d^d ≡ d (mod n) and (-d)^d ≡ d (mod
 *            n)}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* first divisor i */
            idx_t d1 = i;
            seq_t dmod1 = (seq_t)d1 % (seq_t)n;
            seq_t pow1  = mod_pow((seq_t)d1, d1, (seq_t)n);
            if (pow1 == dmod1)
            {
                if ((d1 & 1) == 0) /* even divisor */
                {
                    ++count;
                }
                else
                {
                    seq_t inv1 = ((seq_t)n - pow1) % (seq_t)n;
                    if (inv1 == dmod1)
                        ++count;
                }
            }

            /* second divisor n/i if distinct */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                seq_t dmod2 = (seq_t)d2 % (seq_t)n;
                seq_t pow2  = mod_pow((seq_t)d2, d2, (seq_t)n);
                if (pow2 == dmod2)
                {
                    if ((d2 & 1) == 0)
                    {
                        ++count;
                    }
                    else
                    {
                        seq_t inv2 = ((seq_t)n - pow2) % (seq_t)n;
                        if (inv2 == dmod2)
                            ++count;
                    }
                }
            }
        }
    }
    return count;
}
