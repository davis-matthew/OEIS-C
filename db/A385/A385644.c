/*
 * A385644  Swap multiplication and exponentiation in the canonical prime
 *          factorization of n.
 * https://oeis.org/A385644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (p1*e1)^(p2*e2)^(... )^(pr*er), right associative
 *  where n = p1^e1 * ... * pr^er   (p1 < ... < pr)  */

/*  Implementation note:  The helper functions `factor_u64` and `big_pow`
 *  are supplied by the build system.  `factor_u64` produces the prime
 *  factorization of the input n, and `big_pow` performs exact exponentiation
 *  in the type `seq_t`.                                 */

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) /* a(1) = 1 */
        return 1;

    /* factorial buffers: maximum 64 distinct prime factors for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factorization of n */
    factor_u64((uint64_t)n, primes, exps, &cnt);
    if (cnt == 0) /* n == 1 handled above, but guard anyway */
        return 1;

    /* store p*e values */
    seq_t *pe = (seq_t *)malloc(cnt * sizeof(seq_t));
    for (size_t i = 0; i < cnt; ++i)
        pe[i] = (seq_t)primes[i] * exps[i];

    /* compute right‑associative power, result = pe[0] ^ pe[1] ^ ... ^ pe[cnt‑1] */
    seq_t result = pe[cnt - 1];
    for (int i = (int)cnt - 2; i >= 0; --i)
        result = big_pow(pe[i], result);

    free(pe);
    return result;
}
