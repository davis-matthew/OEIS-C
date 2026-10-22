/*
 * A399736  The sum of the unitary divisors d of n such that n/d is an
 *          exponentially odd number (A268335).
 * https://oeis.org/A399736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e | n} (e mod 2 ? p^e + 1 : p^e)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative with a(p^e) = p^e + 1 if e odd, a(p^e) = p^e if e even. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is indexed from 1; for 1 the product is empty, giving 1. */
    if (n == 1) {
        return (seq_t)1;
    }

    uint64_t nn = (uint64_t)n;
    /* Max distinct primes for 64‑bit integer is 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pcnt = 0;

    /* factor_u64 is provided by the utils library. */
    factor_u64(nn, primes, exps, &pcnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];

        /* Compute p^e as seq_t. */
        seq_t pe = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j) {
            pe *= (seq_t)p;
        }

        if (e & 1U) {          /* odd exponent */
            result *= (pe + (seq_t)1);
        } else {               /* even exponent */
            result *= pe;
        }
    }

    return result;
}
