/*
 * A365491  The number of divisors of the smallest number whose 4th power is
 *          divisible by n.
 * https://oeis.org/A365491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of divisors of the smallest number whose 4th power
 *            is divisible by n. Multiplicative: a(p^e) = ceil(e/4)+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9240 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: compute the local contribution for a prime exponent e. */
static seq_t part_from_exponent(idx_t e)
{
    /* ceil(e/4)+1 = (e+3)/4 + 1 */
    return (seq_t)((e + 3) / 4 + 1);
}

/* Main function. */
seq_t a(idx_t n)
{
    if (n == 0) /* not defined, but harmless for safety */
        return 0;

    seq_t result = 1;
    idx_t temp   = n;
    idx_t p      = 2;

    /* Trial division up to sqrt(temp).  Use p <= temp/p to avoid overflow. */
    while (p <= temp / p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            result *= part_from_exponent(e);
        }
        ++p;
    }

    /* If what remains is a prime > sqrt(original n). */
    if (temp > 1)
    {
        /* Remaining prime has exponent 1. */
        result *= part_from_exponent(1);
    }

    return result;
}
