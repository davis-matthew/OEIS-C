/*
 * A379141  If n = Product (p_j^k_j) then a(n) = numerator of Sum 1/k_j.
 * https://oeis.org/A379141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the reduced sum ∑ 1/k_j, where n = ∏ p_j^k_j
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors n by trial division, builds the rational sum
   of the reciprocals of the exponents, reduces it with the Euclidean
   GCD, and returns the numerator.  All arithmetic is performed in
   the seq_t type as required. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"          /* provides gcd(seq_t,seq_t) */

seq_t a(idx_t n)
{
    /* n = 1 has no prime factors, sum is 0/1 ⇒ numerator 0 */
    if (n <= 1)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    uint64_t ex[64];   /* store exponents k_j */
    size_t    cnt = 0; /* number of distinct primes found */

    /* Trial division factorisation (sufficient for n <= 10000) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t k = 0;
            while (m % p == 0) {
                m /= p;
                ++k;
            }
            ex[cnt++] = k;
        }
    }
    if (m > 1)              /* remaining prime factor */
        ex[cnt++] = 1;

    /* Build the reduced fraction ∑ 1/k_j */
    seq_t num = 0;  /* numerator */
    seq_t den = 1;  /* denominator */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t k = (seq_t)ex[i];
        seq_t new_num = num * k + den;  /* (num/den) + 1/k = (num*k + den) / (den*k) */
        seq_t new_den = den * k;
        seq_t g = gcd(new_num, new_den); /* Euclidean GCD for seq_t */
        num = new_num / g;
        den = new_den / g;
    }

    return num;   /* numerator of the reduced sum */
}
