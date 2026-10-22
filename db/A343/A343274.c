/*
 * A343274  a(n) = Sum_{d|n} d^d * sopf(d).
 * https://oeis.org/A343274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d * sopf(d) where sopf(d) is the sum of
 *            distinct prime factors of d
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"

static uint64_t *spf = NULL;
static idx_t spf_size = 0;

/* Ensure smallest prime factor table up to n (inclusive) */
static void ensure_spf(idx_t n)
{
    idx_t needed = n + 1;
    if (spf_size >= needed)
        return;

    uint64_t *new_spf = (uint64_t *)malloc(needed * sizeof(uint64_t));
    if (!new_spf)
        exit(1);

    /* initialise with identity */
    for (idx_t i = 0; i < needed; ++i)
        new_spf[i] = i;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * i < needed; ++i) {
        if (new_spf[i] == i) {            /* i is prime */
            for (idx_t j = i * i; j < needed; j += i) {
                if (new_spf[j] == j)
                    new_spf[j] = i;
            }
        }
    }

    /* copy old data if any */
    if (spf) {
        memcpy(new_spf, spf, spf_size * sizeof(uint64_t));
        free(spf);
    }

    spf     = new_spf;
    spf_size = needed;
}

/* recursive enumeration of all divisors of n, accumulating the sum */
static void dfs_sum(int idx, seq_t cur_d, seq_t cur_sopf,
                    const uint64_t *pr, const uint32_t *ex, size_t cnt,
                    seq_t *res)
{
    if (idx == (int)cnt) {
        seq_t powd = big_pow(cur_d, cur_d);
        *res += powd * cur_sopf;
        return;
    }

    uint64_t p = pr[idx];
    seq_t   pseq = (seq_t)p;
    seq_t   pow  = 1;

    for (uint32_t e = 0; e <= ex[idx]; ++e) {
        seq_t sopf_e = cur_sopf;
        if (e > 0)
            sopf_e += pseq;                 /* distinct prime factor added once */

        dfs_sum(idx + 1, cur_d * pow, sopf_e, pr, ex, cnt, res);
        pow *= pseq;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_spf(n);

    /* factor n */
    uint64_t primes[64];
    uint32_t exps[64];
    size_t   cnt = 0;
    idx_t    tmp = n;
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        uint32_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt]   = e;
        ++cnt;
    }

    seq_t result = 0;
    dfs_sum(0, (seq_t)1, (seq_t)0, primes, exps, cnt, &result);
    return result;
}
