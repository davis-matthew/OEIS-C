/*
 * A351302  a(n) = n^7 * Product_{p|n, p prime} (1 + 1/p^7).
 * https://oeis.org/A351302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^7 * Product_{p|n} (1 + 1/p^7)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative with a(p^e) = p^(7e) + p^(7e-7) */

#include <stddef.h>              /* size_t */
#include "../utils/factor_u64.h" /* factor_u64 */
#include "../utils/pow_uint.h"   /* pow_uint */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* defined only for n ≥ 1 */

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;                  /* product over prime powers */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* compute p^(7e) and p^(7(e‑1))  (e ≥ 1) */
        idx_t exp1 = 7 * e;                    /* 7e      */
        idx_t exp2 = 7 * (e - 1);              /* 7(e-1)  */

        seq_t term1 = pow_uint(p,   exp1);     /* p^(7e)   */
        seq_t term2 = pow_uint(p,   exp2);     /* p^(7e-7) */

        result *= (term1 + term2);            /* combine the two contributions */
    }

    return result;
}
