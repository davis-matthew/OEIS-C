/*
 * A353783  a(n) = LCM_{p^e||n} sigma(p^e), where n = Product_{p^e||n}, with
 *          each p^e the maximal power of prime p that divides n.
 * https://oeis.org/A353783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM_{p^e||n} sigma(p^e)
 * Range:     u8 n <= 16, u16 n <= 1764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where sigma(p^e) = (p^(e+1)-1)/(p-1) */

#include <stdint.h>
#include <stddef.h>

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint64_t num = (uint64_t)n;
    seq_t lcm = 1;           /* LCM starts at 1 (empty set case: n=1) */

    uint64_t temp = num;
    /* trial division to factor num */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* compute sigma(p^e) = (p^(e+1)-1)/(p-1) */
            seq_t power = 1;
            for (uint64_t i = 0; i < e + 1; ++i) {
                power = power * (seq_t)p;
            }
            seq_t sigma = (power - 1) / (seq_t)(p - 1);

            seq_t g = gcd_seq_t(lcm, sigma);
            lcm = (lcm / g) * sigma;
        }
    }
    /* if a prime factor > sqrt remains */
    if (temp > 1) {
        uint64_t p = temp;      /* exponent e = 1 */
        seq_t power = (seq_t)p * (seq_t)p;   /* p^(1+1) */
        seq_t sigma = (power - 1) / (seq_t)(p - 1);   /* (p^2-1)/(p-1) = p+1 */

        seq_t g = gcd_seq_t(lcm, sigma);
        lcm = (lcm / g) * sigma;
    }

    return lcm;
}
