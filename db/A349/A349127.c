/*
 * A349127  Möbius transform of A064989, where A064989 is multiplicative with
 *          a(2^e) = 1 and a(p^e) = prevprime(p)^e for odd primes p.
 * https://oeis.org/A349127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   * a(n) = 0 if n is even * a(n) = Π_{p^e‖n, p odd} (prevprime(p)
 *            − 1) · prevprime(p)^(e−1) otherwise * where prevprime(p) is the
 *            largest prime < p. * This is the Möbius transform of A064989
 *            with a(2^e)=0
 * Range:     u8 n <= 262, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 111 terms through n = 9895 (2026-11-01)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper functions provided by the build environment. */
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    /* Even indices give 0 immediately. */
    if ((n & 1ULL) == 0)
        return 0;

    /* Factorise n into prime powers.  64 slots are enough for any 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t    cnt   = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        /* p == 2 cannot occur for odd n, but guard anyway. */
        if (p == 2ULL)
            return 0;

        idx_t q = prev_prime((idx_t)p);           /* prevprime(p) */

        /* Compute q^(e-1).  e >= 1. */
        size_t e   = (size_t)exps[i];
        seq_t powq = 1;
        for (size_t j = 1; j < e; ++j)
            powq *= (seq_t)q;

        /* term = (q-1) * q^(e-1) */
        seq_t term = ((seq_t)(q - 1)) * powq;
        prod *= term;
    }
    return prod;
}
