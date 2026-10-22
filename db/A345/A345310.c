/*
 * A345310  a(n) = Sum_{p|n, p prime} p^gcd(p,n/p).
 * https://oeis.org/A345310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^gcd(p,n/p)
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1U)
        return 0;                     /* a(1) = 0 */

    seq_t sum = 0;
    uint64_t original_n = (uint64_t)n;
    uint64_t temp = original_n;

    /* factor by trial division */
    for (uint64_t p = 2; p <= original_n / p; ++p) {
        if (temp % p == 0U) {
            /* compute power for this prime divisor */
            uint64_t m = original_n / p;
            seq_t exp = gcd((seq_t)p, (seq_t)m);
            seq_t term = big_pow((seq_t)p, exp);
            sum += term;

            /* strip all powers of p from temp */
            while (temp % p == 0U)
                temp /= p;
        }
    }

    /* if a prime factor remains > sqrt(original_n) */
    if (temp > 1U) {
        uint64_t p = temp;
        uint64_t m = original_n / p;
        seq_t exp    = gcd((seq_t)p, (seq_t)m);
        seq_t term   = big_pow((seq_t)p, exp);
        sum += term;
    }

    return sum;
}
