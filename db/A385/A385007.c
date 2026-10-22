/*
 * A385007  The largest unitary divisor of n that is a biquadratefree number
 *          (A046100).
 * https://oeis.org/A385007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e of (p^e if e <= 3 else 1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;   /* work with a standard 64‑bit integer */
    seq_t res = 1;              /* result accumulator */

    /* Handle factor 2 separately */
    uint64_t exp2 = 0;
    while ((m & 1) == 0)
    {
        m >>= 1;
        ++exp2;
    }
    if (exp2 > 0 && exp2 <= 3)
    {
        seq_t mult = 1;
        for (uint64_t i = 0; i < exp2; ++i) mult *= (seq_t)2;
        res *= mult;
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
        {
            uint64_t cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
            }
            if (cnt <= 3)
            {
                seq_t mult = 1;
                for (uint64_t i = 0; i < cnt; ++i) mult *= (seq_t)p;
                res *= mult;
            }
        }
    }

    /* Any remaining prime factor (exponent 1) */
    if (m > 1)
    {
        res *= (seq_t)m;
    }

    return res;
}
