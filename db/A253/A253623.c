/*
 * A253623  Expansion of phi(q) * f(q, q^2)^2 / f(q^2, q^4) in powers of q
 *          where phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A253623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * b(n), where b(n) is multiplicative with the following
 *            rule: * b(2^e) = (3/4)*(1 - (-1)^e) if e > 0 * b(3^e) = 1 * For
 *            prime p ≡ 1 (mod 6), b(p^e) = e + 1 * For prime p ≡ 5 (mod 6),
 *            b(p^e) = (1 + (-1)^e)/2 * Then a(n) = 4 * b(n)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* offset = 0 */

    uint64_t nn = (uint64_t)n;

    /* Factor n into primes.  The number of distinct prime factors of a 64‑bit
       integer is at most 16, but we allocate a small buffer for safety. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* Check for zero cases and whether 2 appears with an odd exponent. */
    int zero        = 0;
    int two_odd     = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            if ((e & 1) == 0 && e > 0) {  /* even exponent > 0 → factor 0 */
                zero = 1;
                break;
            }
            if (e & 1) two_odd = 1;         /* odd exponent → factor 3/2 later */
        } else if (p == 3) {
            /* factor 1, no effect */
        } else if (p % 6 == 5) {
            if ((e & 1) == 1) {            /* odd exponent → factor 0 */
                zero = 1;
                break;
            }
        } else if (p % 6 == 1) {
            /* factor (e + 1) will be multiplied into the product below */
        } else { /* other primes (should not happen) */
            /* treat as factor 1; safe */
        }
    }

    if (zero) return (seq_t)0;

    /* Compute the integer part of b(n).  For p ≡ 1 (mod 6) we multiply by (e+1).  */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2 || p == 3) continue;
        if (p % 6 == 1) {
            prod *= (seq_t)(e + 1);
        }
        /* p ≡ 5 (mod 6) with even exponent contributes factor 1 – nothing to do. */
    }

    /* Translate b(n) into a(n)
       • if 2^odd is present:  a(n) = 4 * (prod * 3/2) = 6 * prod
       • otherwise:            a(n) = 4 * prod   */
    seq_t result = (two_odd ? prod * (seq_t)6 : prod * (seq_t)4);
    return result;
}
