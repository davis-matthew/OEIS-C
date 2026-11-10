/*
 * A374031  a(n) = gcd(A276085(n), A328768(n)), where A276085 is the primorial
 *          base log-function, and A328768 is the first primorial based
 *          variant of arithmetic derivative.
 * https://oeis.org/A374031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A276085(n), A328768(n)), where A276085(n) = Sum_{p^e
 *            || n} e*(p#/p) and A328768(n) = Sum_{p^e || n} e*(n/p)*(p#/p),
 *            with p# the product of primes <= p; a(1) = 0
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 58, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t A374031_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t facs[64];
    uint64_t exps[64];
    size_t fcnt = 0;
    factor_u64((uint64_t)n, facs, exps, &fcnt);

    seq_t s1 = 0; /* A276085 */
    seq_t s2 = 0; /* A328768 */
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = facs[i];
        seq_t P = 1; /* product of primes < p */
        for (uint64_t q = 2; q < p; q = next_prime(q))
            P *= (seq_t)q;
        seq_t e = (seq_t)exps[i];
        s1 += e * P;
        s2 += ((seq_t)n / (seq_t)p) * e * P;
    }
    return A374031_gcd(s1, s2);
}
