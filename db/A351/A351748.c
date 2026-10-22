/*
 * A351748  a(n) = Sum_{p|n, p prime} n^gcd(p,n/p).
 * https://oeis.org/A351748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} n^gcd(p,n/p)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 48, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

/* Compute A351748 for an index n (n >= 1) */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1; a(1) = 0 */
    if (n <= 1)
        return 0;

    seq_t sum = 0;                /* accumulating result */
    seq_t base = (seq_t)n;        /* base for exponentiation */
    uint64_t m = (uint64_t)n;     /* copy used for trial division */

    /* trial division over all prime divisors of n */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {                /* p is a prime factor */
            /* contribution of this prime */
            uint64_t rest = (uint64_t)n / p;
            seq_t g = gcd((seq_t)p, (seq_t)rest);   /* gcd(p, n/p) */
            sum += big_pow(base, g);

            /* remove all copies of the factor p from m */
            while (m % p == 0)
                m /= p;
        }
    }

    /* any remaining prime factor larger than sqrt(n) */
    if (m > 1) {
        uint64_t p = m;
        uint64_t rest = (uint64_t)n / p;
        seq_t g = gcd((seq_t)p, (seq_t)rest);
        sum += big_pow(base, g);
    }

    return sum;
}
