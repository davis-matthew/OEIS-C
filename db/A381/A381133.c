/*
 * A381133  If n = (p_1^e_1)*(p_2^e_2)*(p_3^e_3)*... and
 *          min(p_1^e_1,p_2^e_2,...) = p_k^e_k then a(n) = e_k, a(1) = 0.
 * https://oeis.org/A381133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 1, factor n = Π p_i^e_i. Let p_k^e_k be the prime‑power
 *            divisor of n with the smallest numerical value. Then a(n) = e_k.
 *            a(1) = 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) as defined above */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t t = (uint64_t)n;
    seq_t min_exp = 0;
    seq_t min_val  = 0;   /* Will be set on first prime‑power found */
    bool     first  = true;

    /* Trial division over all possible prime factors up to sqrt(t) */
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            seq_t pow_val = 1;   /* p^e */
            uint64_t cnt  = 0;

            while (t % p == 0)
            {
                t /= p;
                pow_val *= (seq_t)p;
                ++cnt;
            }

            if (first || pow_val < min_val)
            {
                min_val = pow_val;
                min_exp = cnt;
                first   = false;
            }
        }
    }

    /* Any remaining prime factor > sqrt(t) contributes exponent 1 */
    if (t > 1)
    {
        seq_t pow_val = (seq_t)t; /* remaining prime factor with exponent 1 */

        if (first || pow_val < min_val)
        {
            min_val = pow_val;
            min_exp = 1;
            first   = false;
        }
    }

    return min_exp;
}
