/*
 * A345312  a(n) = Sum_{p|n} (n/p)^gcd(p,n/p).
 * https://oeis.org/A345312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} (n/p)^gcd(p,n/p)
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 120, u64 n <= 288,
 *            u128 n <= 840, big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* if n == 1, there are no prime divisors → sum remains 0 */
    if (n == 1)
        return sum;

    idx_t temp = n;                /* remaining part to factor */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* compute (n/p)^gcd(p, n/p) */
            seq_t base = (seq_t)(n / p);
            seq_t g     = gcd((seq_t)p, base);
            sum += big_pow(base, g);
            /* remove all powers of p from temp */
            while (temp % p == 0)
                temp /= p;
        }
    }
    /* if a prime factor larger than sqrt(n) remains */
    if (temp > 1) {
        seq_t base = (seq_t)(n / temp);
        seq_t g    = gcd((seq_t)temp, base);
        sum += big_pow(base, g);
    }
    return sum;
}
