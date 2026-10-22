/*
 * A354094  a(n) = phi(A354091(n)), where A354091 is fully multiplicative
 *          prime shift which replaces the primes of the form 3k+2 by the next
 *          larger such prime, while other primes stay as they are, and phi is
 *          Euler totient function.
 * https://oeis.org/A354094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = φ(A354091(n)) where A354091 is the fully multiplicative
 *            prime shift that replaces each prime p ≡ 2 (mod 3) by the next
 *            larger prime also ≡ 2 (mod 3); all other primes remain unchanged
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility functions are provided by the build. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t) */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t, idx_t) */

/* Map a prime p to the adjusted prime q for A354091. */
static uint64_t map_prime(uint64_t p)
{
    /* primes not congruent to 2 mod 3 stay unchanged */
    if (p % 3U != 2U)
        return p;

    /* find the next prime > p that is ≡ 2 (mod 3) */
    uint64_t q = next_prime(p);
    while (q % 3U != 2U)
        q = next_prime(q);
    return q;
}

/* Compute a(n) = φ(A354091(n)) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* values for n < 1 are undefined; return 0 as a safe default */

    /* factor n; enough space for all prime factors of a 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = map_prime(p);

        /* φ(q^e) = (q-1) * q^(e-1)  for e ≥ 1 */
        seq_t term = (seq_t)(q - 1U);
        if (e > 1)
            term *= seq_pow((seq_t)q, (idx_t)(e - 1));
        result *= term;
    }
    return result;
}
