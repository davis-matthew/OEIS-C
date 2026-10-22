/*
 * A359819  Dirichlet inverse of A359590.
 * https://oeis.org/A359819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative definition of A359819:
   a(1) = 1
   a(2^e) = 0 if e=1
   a(2^e) = -1 if e≡2 or 3 (mod 4)
   a(2^e) =  1 if e≡0 or 1 (mod 4)   (e>1)
   a(p^e) = (-1)^e for every odd prime p
   For general n, a(n) = ∏ a(p^e) over prime powers of n. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* local arrays big enough for factorization of 64‑bit numbers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            if (e == 1)
                return (seq_t)0;          /* a(2) = 0 */
            uint64_t mod4 = e & 3ULL;     /* e % 4 */
            if (mod4 == 2 || mod4 == 3)   /* -1 for e≡2,3 (mod 4) */
                result = -result;
            /* else multiply by +1: nothing changes */
        } else { /* odd prime */
            if (e & 1ULL)                  /* odd exponent -> factor -1 */
                result = -result;
        }
    }

    return result;
}
