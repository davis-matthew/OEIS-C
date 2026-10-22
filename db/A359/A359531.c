/*
 * A359531  a(1) = 1, a(2) = -9; a(n) = -n^3 * Sum_{d|n, d < n} a(d) / d^3.
 * https://oeis.org/A359531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative. For a prime factorisation n = ∏ pᶦˢ, we
 *            have a(1) = 1 a(2) = –9, a(4) = 8, a(2ᵉ) = 0 for e > 2 a(p) =
 *            –p³ (p an odd prime) a(pᵉ) = 0 for e > 1 (p odd) Hence a(n) is
 *            the product of the non‑zero a(pᵉ) for the prime powers occurring
 *            in n. This implements the recurrence given...
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1245, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 1)
        return 1;

    /* Factor n.  The helper writes the primes into primes[i]
       and the corresponding exponents into exps[i], with
       *cnt = number of distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   num   = 0;
    factor_u64((uint64_t)n, primes, exps, &num);

    seq_t result = 1;
    for (size_t i = 0; i < num; ++i) {
        seq_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part;

        if (p == 2) {
            if (e == 1)
                part = (seq_t)-9;    /* a(2)   */
            else if (e == 2)
                part = (seq_t)8;     /* a(4)   */
            else
                part = 0;            /* a(2ᵉ)=0 for e>2 */
        } else { /* odd prime */
            if (e == 1) {
                seq_t pp = p;
                pp *= pp;    /* p² */
                pp *= p;     /* p³ */
                part = -pp;  /* a(p)          */
            } else {
                part = 0;    /* a(pᵉ)=0 for e>1 */
            }
        }

        result *= part;
        if (result == 0)          /* zero factor kills the product */
            return 0;
    }
    return result;
}
