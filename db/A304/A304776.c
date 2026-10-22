/*
 * A304776  A weakening function. a(n) = n / A007947(n)^(A051904(n) - 1) where
 *          A007947 is squarefree kernel and A051904 is minimum prime
 *          exponent.
 * https://oeis.org/A304776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / A007947(n)^(A051904(n)-1)
 * Range:     u8 n <= 256, u16 n <= 9993, u32 n <= 9993, u64 n <= 9993,
 *            u128 n <= 9993, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset.  a(1) = 1 by definition. */
    if (n == 1)
        return 1;
    if (n == 0)
        return 0;  /* below offset – behaviour irrelevant */

    /* Factor n.  Since n ≤ 10 000 for required test ranges,
       at most 15 distinct primes can divide it. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;              /* will be filled by factor_u64 */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Find the minimum exponent among the prime factors. */
    uint64_t min_e = UINT64_MAX;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] < min_e)
            min_e = exps[i];
    }

    /* Compute a(n) = ∏ p_i^(e_i - min_e + 1). */
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i] - min_e + 1;      /* exponent ≥ 1       */
        res *= big_pow((seq_t)primes[i], (seq_t)e);
    }
    return res;
}
