/*
 * A288143  Expansion of x * phi(x) * phi(x^3)^2 * f(x, x^5)^3 in powers of x
 *          where phi() is a Ramanujan theta function and f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A288143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with * a(3^e) = 9^e, * a(2^e) =
 *            (4^(e+1)+9*(-1)^(e+1))/5 (e>0), * a(p^e) =
 *            ((p^2)^(e+1)-1)/(p^2-1) if p≡1 (mod 6), * a(p^e) =
 *            ((p^2)^(e+1)-(-1)^(e+1))/(p^2+1) if p≡5 (mod 6)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 508, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

static seq_t a_prime_power(uint64_t p, unsigned e)
{
    if (p == 3u) {
        return pow_int((seq_t)9, e);
    }
    if (p == 2u) {
        seq_t t1 = pow_int((seq_t)4, e + 1);
        seq_t numerator = (((e + 1) & 1) == 0) ? t1 + (seq_t)9
                                                : t1 - (seq_t)9;
        return numerator / (seq_t)5;
    }
    seq_t p_sq = (seq_t)p * (seq_t)p;
    if (p % 6u == 1u) {
        seq_t pow_val = pow_int(p_sq, e + 1);
        seq_t numerator = pow_val - (seq_t)1;
        seq_t denominator = p_sq - (seq_t)1;
        return numerator / denominator;
    } else { /* p % 6 == 5 */
        seq_t pow_val = pow_int(p_sq, e + 1);
        seq_t numerator = (((e + 1) & 1) == 0) ? pow_val - (seq_t)1
                                                : pow_val + (seq_t)1;
        seq_t denominator = p_sq + (seq_t)1;
        return numerator / denominator;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    /* Handle factor 2 */
    unsigned e2 = 0;
    while ((temp & 1u) == 0) {
        ++e2;
        temp >>= 1;
    }
    if (e2) result *= a_prime_power(2u, e2);

    /* Handle odd prime factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= a_prime_power(p, exp);
        }
    }

    /* Remaining prime > sqrt */
    if (temp > 1) {
        result *= a_prime_power(temp, 1);
    }

    return result;
}
