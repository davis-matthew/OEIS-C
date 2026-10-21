/*
 * A252096  Largest prime divisor of n^2+1 - smallest prime divisor of n^2+1.
 * https://oeis.org/A252096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest_prime_divisor(n^2+1) -
 *            smallest_prime_divisor(n^2+1)
 * Range:     u8 n <= 24, u16 n <= 370, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 per OEIS offset; guard only in case of misuse */
    if (n < 1) return 0;

    /* Compute n^2 + 1 in the widest type we need */
    uint64_t mp = (uint64_t)n * (uint64_t)n + 1;

    /* Factor mp by trial division: store smallest and largest prime factor */
    uint64_t sp = 0;          /* smallest prime factor of mp */
    uint64_t lp = 0;          /* largest prime factor of mp    */
    uint64_t temp = mp;
    uint64_t p = 2;

    /* Trial division up to sqrt(temp) */
    while (p <= temp / p) {
        if (temp % p == 0) {
            if (sp == 0) sp = p;  /* first factor encountered */
            lp = p;               /* last factor seen so far is the current largest */
            while (temp % p == 0) temp /= p;  /* remove all copies of p */
        }
        /* After checking 2, skip even numbers */
        if (p == 2) p = 3;
        else p += 2;
    }

    /* If anything remains, it is a prime greater than sqrt(original mp) */
    if (temp > 1) {
        if (sp == 0) sp = temp;       /* mp itself was prime */
        if (temp > lp) lp = temp;     /* it is the largest prime factor */
    }

    /* Difference of largest and smallest prime factors */
    seq_t result = (seq_t)(lp - sp);
    return result;
}
