/*
 * A368738  a(n) = Sum_{k = 1..n} gcd(3*k + 1, n).
 * https://oeis.org/A368738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k = 1..n} gcd(3*k + 1, n)
 * Range:     u8 n <= 55, u16 n <= 4479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the multiplicative property:   */
/*   a(3^k) = 3^k                         */
/*   a(p^k) = (k + 1) * p^k - k * p^(k-1)  for p ≠ 3 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n = 0; safe guard */

    seq_t result = 1;
    idx_t temp = n;

    /* factor n by primes up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0) continue;

        idx_t e = 0;
        while (temp % p == 0) {
            ++e;
            temp /= p;
        }

        /* compute p^e in seq_t */
        seq_t pow_e = 1;
        for (idx_t i = 0; i < e; ++i) {
            pow_e *= (seq_t)p;
        }

        seq_t term;
        if (p == 3) {
            /* a(3^e) = 3^e */
            term = pow_e;
        } else {
            /* a(p^e) = (e + 1) * p^e - e * p^(e-1) */
            seq_t pow_e_minus1 = pow_e / (seq_t)p; /* e ≥ 1, safe division */
            term = ((seq_t)(e + 1)) * pow_e - ((seq_t)e) * pow_e_minus1;
        }

        result *= term;
    }

    /* remaining prime factor (> sqrt(original n)) */
    if (temp > 1) {
        idx_t p = temp;      /* exponent e = 1 */
        seq_t pow_e = (seq_t)p;

        seq_t term;
        if (p == 3) {
            term = pow_e;          /* a(3) = 3 */
        } else {
            /* a(p^1) = 2*p - 1 */
            term = ((seq_t)2) * pow_e - (seq_t)1;
        }

        result *= term;
    }

    return result;
}
