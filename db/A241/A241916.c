/*
 * A241916  a(2^k) = 2^k, and for other numbers, if n = 2^e1 * 3^e2 * 5^e3 *
 *          ... p_k^e_k, then a(n) = 2^(e_k - 1) * 3^(e_{k-1}) * ... *
 *          p_{k-1}^e2 * p_k^(e1+1). Here p_k is the greatest prime factor of
 *          n (A006530), and e_k is its exponent (A071178), and the exponents
 *          e1, ..., e_{k-1} >= 0.
 * https://oeis.org/A241916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(e_k-1) * 3^(e_{k-1}) * ... * p_{k-1}^{e2} *
 *            p_k^((e_1)+1)
 * Range:     u8 n <= 27, u16 n <= 135, u32 n <= 1311, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   build_primes_upto, factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here e_1 is the exponent of the prime 2 (0 if 2∤n), e_k the exponent of the
   largest prime factor p_k of n, and the exponents e_i correspond to the primes
   in ascending order up to p_k. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Powers of two are fixed points */
    if ((n & (n - 1)) == 0)
        return (seq_t)n;

    /* Factor n */
    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt_f = 0;
    factor_u64(n, primes_f, exps_f, &cnt_f);

    /* Largest prime factor and its exponent */
    uint64_t p_k = primes_f[cnt_f - 1];
    uint64_t e_k = exps_f[cnt_f - 1];

    /* Exponent of 2 (may be zero if 2∤n) */
    uint64_t e_1 = 0;
    for (size_t i = 0; i < cnt_f; ++i)
    {
        if (primes_f[i] == 2)
        {
            e_1 = exps_f[i];
            break;
        }
    }

    /* Build the full list of primes up to p_k (inclusive) */
    uint64_t *primes_all = NULL;
    size_t cnt_all = 0;
    build_primes_upto(p_k, &primes_all, &cnt_all);

    seq_t result = 1;

    /* Exponent for prime 2 */
    if (e_k > 1)
        result *= seq_pow((seq_t)2, (idx_t)(e_k - 1));

    /* Exponents for intermediate primes */
    for (size_t j = 1; j + 1 < cnt_all; ++j)
    {
        uint64_t target = primes_all[j];
        /* source prime index is mirrored: cnt_all-1-j */
        uint64_t source = primes_all[cnt_all - 1 - j];

        /* Find exponent of source prime in n */
        uint64_t exp_src = 0;
        for (size_t k = 0; k < cnt_f; ++k)
        {
            if (primes_f[k] == source)
            {
                exp_src = exps_f[k];
                break;
            }
        }

        if (exp_src > 0)
            result *= seq_pow((seq_t)target, (idx_t)exp_src);
    }

    /* Exponent for largest prime (p_k) */
    seq_t exp_p_k = e_1 + 1;
    if (exp_p_k > 0)
        result *= seq_pow((seq_t)p_k, (idx_t)exp_p_k);

    free(primes_all);
    return result;
}
