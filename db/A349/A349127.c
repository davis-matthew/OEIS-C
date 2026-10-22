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
 * Range:     u8 n <= 262, u16 n <= 9895, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   factor_u64, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper functions provided by the build environment. */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/prev_prime.h"   /* idx_t prev_prime(idx_t n); */

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
