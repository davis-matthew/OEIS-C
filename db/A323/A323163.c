/*
 * A323163  Greatest common divisor of product (1+(p^e)) and product p^(e-1),
 *          where p ranges over prime factors of n, with e corresponding
 *          exponent; a(n) = gcd(A034448(n), A003557(n)).
 * https://oeis.org/A323163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003557(n), A034448(n))
 * Range:     u8 n <= 24499, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9851 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = gcd(product of p^(e-1), product of (1 + p^e)) for the prime factorization n = ∏ p^e */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n into primes and exponents */
    factor_u64(n, primes, exps, &cnt);

    seq_t prod1 = (seq_t)1; /* ∏ (1 + p^e) */
    seq_t prod2 = (seq_t)1; /* ∏ p^(e-1) */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t e   = (seq_t)exps[i];

        seq_t p_pow_e   = ipow(p, e);          /* p^e */
        prod1 *= (p_pow_e + (seq_t)1);            /* (1 + p^e) */

        seq_t p_pow_e_1 = ipow(p, e - (seq_t)1);/* p^(e-1) */
        prod2 *= p_pow_e_1;
    }

    return gcd(prod1, prod2);
}
