/*
 * A278085  1/4 of the number of primitive integral quadruples with sum = 3*n
 *          and sum of squares = 3*n^2.
 * https://oeis.org/A278085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ p^e∥n p^(e-1)·a(p) with a(2)=1, a(2^k)=0 for k≥2,
 *            a(3)=3, a(p)=p+1 if p≡5 (mod 6) (odd prime), a(p)=p-1 if p≡1
 *            (mod 6) (odd prime)
 * Range:     u8 n <= 224, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t remaining = n;

    /* Handle factor 2 */
    if ((remaining & 1u) == 0u) {
        size_t e = 0;
        while ((remaining & 1u) == 0u) {
            ++e;
            remaining >>= 1u;
        }
        if (e >= 2u)
            return (seq_t)0;   /* 2^k with k≥2 gives 0 */
        /* e == 1 contributes factor 1; nothing to multiply */
    }

    /* Handle odd prime factors by trial division up to sqrt(remaining) */
    for (idx_t p = 3u; p * p <= remaining; p += 2u) {
        if (remaining % p == 0u) {
            size_t e = 0;
            while (remaining % p == 0u) {
                ++e;
                remaining /= p;
            }

            /* Compute p^(e-1) */
            seq_t pow = (seq_t)1;
            for (size_t i = 0; i < e - 1; ++i)
                pow *= (seq_t)p;

            /* Compute a(p) for odd prime p */
            seq_t a_p;
            if (p == 3u)
                a_p = (seq_t)3;
            else if ((p % 6u) == 5u)
                a_p = (seq_t)p + (seq_t)1;
            else
                a_p = (seq_t)p - (seq_t)1;

            result *= pow;
            result *= a_p;
        }
    }

    /* If remaining > 1, it is a prime factor with exponent 1 */
    if (remaining > 1u) {
        seq_t p = (seq_t)remaining;
        seq_t a_p;
        if (remaining == 3u)
            a_p = (seq_t)3;
        else if ((remaining % 6u) == 5u)
            a_p = p + (seq_t)1;
        else
            a_p = p - (seq_t)1;
        result *= a_p;
    }

    return result;
}
