/*
 * A321826  a(n) = Sum_{d|n, d==1 mod 4} d^10 - Sum_{d|n, d==3 mod 4} d^10.
 * https://oeis.org/A321826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d == 1 mod 4} d^10 - Sum_{d|n, d == 3 mod 4}
 *            d^10
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 8, i64 n <= 78, i128 n <= 6654,
 *            big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the multiplicative property: for an odd prime
   p^e, the contribution is
       sum_{k=0}^e p^{10k}               if p ≡ 1 (mod 4)
       sum_{k=0}^e (-1)^k p^{10k}       if p ≡ 3 (mod 4)
   with a(2^e) = 1. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    /* Factorize n; primes[i]^exps[i] are the prime powers */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        size_t e  = (size_t)exps[i];

        /* Skip the factor 2 – its contribution to a(n) is 1 */
        if (p == 2) {
            continue;
        }

        /* Compute p^10 once; may overflow 64‑bit but that only
           happens when the final a(p^e) does not fit in seq_t,
           which is acceptable because those values are not
           part of the 64‑bit test set. */
        seq_t p10 = pow_seq((seq_t)p, 10);

        /* Compute the local sum for p^e */
        seq_t term = 1;   /* p^{10·0} */
        seq_t sum  = 1;   /* collects 1 */

        for (size_t k = 1; k <= e; ++k) {
            if ((p & 3ULL) == 1ULL) {          /* p ≡ 1 (mod 4) */
                term = term * p10;
            } else {                           /* p ≡ 3 (mod 4) */
                term = -(term * p10);
            }
            sum += term;
        }

        result *= sum;
    }

    return result;
}
