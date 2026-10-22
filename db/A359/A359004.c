/*
 * A359004  a(n) = Sum_{d|n} d^(n/d-1) * (n/d)^(d-1).
 * https://oeis.org/A359004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n/d-1) * (n/d)^(d-1)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 44, u64 n <= 101,
 *            u128 n <= 221, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

static void gen_divisors_rec(const uint64_t *primes,
                             const uint64_t *exps,
                             size_t pf_cnt,
                             size_t idx,
                             uint64_t current,
                             uint64_t *out,
                             size_t *out_len)
{
    if (idx == pf_cnt) {
        out[(*out_len)++] = current;
        return;
    }
    uint64_t val = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors_rec(primes, exps, pf_cnt, idx + 1,
                         current * val, out, out_len);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for out‑of‑range */
    /* Factorize n */
    static uint64_t pf_primes[64];
    static uint64_t pf_exps[64];
    static size_t pf_cnt = 0;
    size_t cnt;
    factor_u64((uint64_t)n, pf_primes, pf_exps, &cnt);
    pf_cnt = cnt;

    /* Generate all divisors */
    uint64_t divisors[256];
    size_t dcnt = 0;
    gen_divisors_rec(pf_primes, pf_exps, pf_cnt, 0, (uint64_t)1,
                     divisors, &dcnt);

    /* Compute the sum */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        uint64_t nd = n / d;                   /* integer division since d|n */

        seq_t d_pow  = big_pow((seq_t)d, (seq_t)(nd - 1));
        seq_t nd_pow = big_pow((seq_t)nd, (seq_t)(d - 1));

        sum += d_pow * nd_pow;                 /* multiplication works for seq_t */
    }
    return sum;
}
