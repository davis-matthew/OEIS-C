/*
 * A321815  Sum of 11th powers of odd divisors of n.
 * https://oeis.org/A321815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_11(odd part of n) = product over odd prime powers
 *            p^e dividing n of (p^(11*(e+1)) - 1) / (p^11 - 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 8, u64 n <= 56, u128 n <= 3184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;

    /* Remove all factors of 2: we only need the odd part */
    while ((m & 1) == 0) {
        m >>= 1;
    }

    seq_t result = 1;
    if (m == 1) {
        return result;    /* n is a power of 2 */
    }

    /* Factorise the odd part by trivial trial division.
       For n up to about 10 000 this is fast enough. */
    uint64_t temp = m;

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }

            /* Compute sum_{i=0}^{e} p^{11*i} */
            seq_t pp = 1;                /* p^11 */
            for (int i = 0; i < 11; ++i) {
                pp *= (seq_t)p;
            }

            seq_t term = 1;
            seq_t sum_p = 1; /* i = 0 term */
            for (uint64_t i = 1; i <= e; ++i) {
                term *= pp;    /* term = p^{11*i} */
                sum_p += term; /* accumulate the sum */
            }

            result *= sum_p;
        }
    }

    /* If a prime factor larger than sqrt(m) remains */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t e = 1;   /* exponent is 1 */

        seq_t pp = 1;
        for (int i = 0; i < 11; ++i) {
            pp *= (seq_t)p;
        }

        seq_t term = 1;
        seq_t sum_p = 1;
        for (uint64_t i = 1; i <= e; ++i) {
            term *= pp;
            sum_p += term;
        }

        result *= sum_p;
    }

    return result;
}
