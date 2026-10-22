/*
 * A373440  Denominator of sum of reciprocals of square divisors of n.
 * https://oeis.org/A373440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the sum of reciprocals of square divisors
 *            of n
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 4023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

/* Helper that processes one prime factor p^e and updates the running
   numerator and denominator of the sum. */
static void
process_prime(seq_t *num, seq_t *den, uint64_t p, size_t e)
{
    size_t f = e / 2;                       /* floor(e/2) */
    idx_t exp2 = 2 * (f + 1);               /* exponent for p^{2*(f+1)} */

    seq_t p_pow = pow_uint(p, exp2);        /* p^{2*(f+1)}   */
    seq_t p2    = pow_uint(p, 2);           /* p^2           */
    seq_t temp1 = p_pow - 1;                /* p_pow - 1     */
    seq_t temp2 = p2 - 1;                   /* p2   - 1     */
    seq_t S_num = p2 * temp1;               /* numerator of S_i */
    seq_t S_den = temp2 * p_pow;            /* denominator of S_i */

    /* Reduce crosswise before multiplication to keep numbers small. */
    seq_t g1 = gcd(*num, S_den);
    *num     /= g1;
    S_den     /= g1;

    seq_t g2 = gcd(S_num, *den);
    S_num     /= g2;
    *den       /= g2;

    /* Multiply and simplify. */
    *num *= S_num;
    *den *= S_den;

    seq_t g3 = gcd(*num, *den);
    *num /= g3;
    *den /= g3;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t temp = (uint64_t)n;          /* work with 64‑bit integer for factoring */
    seq_t num = 1;
    seq_t den = 1;

    /* Factorization by trial division (fast enough for n ≤ 10000 and
       reasonable for larger n when called only few times). */
    if (temp % 2 == 0) {
        size_t e = 0;
        while (temp % 2 == 0) { temp /= 2; ++e; }
        process_prime(&num, &den, 2, e);
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            process_prime(&num, &den, p, e);
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        process_prime(&num, &den, temp, 1);
    }

    return den;                /* the denominator in lowest terms */
}
