/*
 * A319690  Fully multiplicative with a(p^e) = (p mod 3)^e.
 * https://oeis.org/A319690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9674, u32 n <= 9674, u64 n <= 9674,
 *            u128 n <= 9674, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Π_{p^e || n} (p mod 3)^e
 *   with a(3^e) = 0 for any e ≥ 1.
 *   Hence a(n) = 0 iff 3 divides n, else a(n) = 2^{Σ_{p≡2 (mod 3)} e_p}.
 */

#include <stdint.h>

/* fully multiplicative: a(p^e) = (p mod 3)^e */
seq_t a(idx_t n)
{
    if (n < 1)                /* below OEIS offset – undefined, return 0 */
        return 0;

    if (n == 1)               /* base case */
        return 1;

    /* if 3 | n, result is 0 */
    if (n % 3 == 0)
        return 0;

    seq_t res = 1;
    idx_t tmp = n;

    /* trial division up to sqrt(tmp) */
    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            if (p % 3 == 2)             /* contributes a factor 2^exp */
            {
                for (idx_t i = 0; i < exp; ++i)
                    res *= 2;
            }
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1)
    {
        if (tmp % 3 == 2)
            res *= 2;                    /* exponent is 1 */
    }

    return res;
}
