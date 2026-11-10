/*
 * A369033  LCM-transform of permutation A241916.
 * https://oeis.org/A369033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = A014963(A241916(n)), where A014963(m) = p if m
 *            is a power of a prime p, else 1
 * Range:     u8 n <= 27, u16 n <= 2904, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9843 (2026-11-10)
 * Helpers:   build_primes_upto, factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

static seq_t A241916_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if ((n & (n - 1)) == 0)
        return (seq_t)n;

    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt_f = 0;
    factor_u64(n, primes_f, exps_f, &cnt_f);

    uint64_t p_k = primes_f[cnt_f - 1];
    uint64_t e_k = exps_f[cnt_f - 1];

    uint64_t e_1 = 0;
    for (size_t i = 0; i < cnt_f; ++i)
    {
        if (primes_f[i] == 2)
        {
            e_1 = exps_f[i];
            break;
        }
    }

    uint64_t *primes_all = NULL;
    size_t cnt_all = 0;
    build_primes_upto(p_k, &primes_all, &cnt_all);

    seq_t result = 1;

    if (e_k > 1)
        result *= ipow_idx((seq_t)2, (idx_t)(e_k - 1));

    for (size_t j = 1; j + 1 < cnt_all; ++j)
    {
        uint64_t target = primes_all[j];
        uint64_t source = primes_all[cnt_all - 1 - j];

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
            result *= ipow_idx((seq_t)target, (idx_t)exp_src);
    }

    seq_t exp_p_k = e_1 + 1;
    if (exp_p_k > 0)
        result *= ipow_idx((seq_t)p_k, (idx_t)exp_p_k);

    free(primes_all);
    return result;
}

static seq_t A014963_a(seq_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t spf = 0;
    for (seq_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            spf = i;
            break;
        }
    }

    if (spf == 0)
        return n;

    seq_t t = n;
    while (t % spf == 0)
        t /= spf;

    if (t == 1)
        return spf;
    return (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    return A014963_a(A241916_a(n));
}
