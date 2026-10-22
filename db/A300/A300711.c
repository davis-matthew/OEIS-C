/*
 * A300711  a(n) = A000367(n)/A001067(n).
 * https://oeis.org/A300711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product over all prime powers p^e dividing 2n
 *               where p-1 does not divide 2n.
 *               (equivalently gcd(numerator(Bernoulli(2n)), 2n))
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for n = 0, but keep a harmless return     */
        return 0;

    seq_t result = 1;   /* running product */

    /* factor n; 2 is handled specially (and always omitted in the result) */
    idx_t temp = n;
    while ((temp & 1U) == 0)     /* remove all factors of 2 */
        temp >>= 1;

    /* trial division for odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p != 0)
            continue;

        /* exponent of p in n (hence in 2n) */
        unsigned int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* the condition: (p-1) does NOT divide 2n */
        idx_t mod = p - 1;            /* p > 2, so mod > 0 */
        idx_t n_mod = n % mod;
        idx_t two_n_mod = (n_mod * 2) % mod;   /* (2n) mod (p-1) */

        if (two_n_mod != 0) {        /* include p^exp in the product */
            seq_t powp = 1;
            for (unsigned int i = 0; i < exp; ++i)
                powp *= (seq_t)p;
            result *= powp;
        }
    }

    /* any remaining prime factor > sqrt(original temp) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t mod = p - 1;
        idx_t n_mod = n % mod;
        idx_t two_n_mod = (n_mod * 2) % mod;

        if (two_n_mod != 0)
            result *= (seq_t)p;          /* exponent is 1 */
    }

    return result;
}
