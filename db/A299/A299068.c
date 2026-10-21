/*
 * A299068  Number of pairs of factors of n^2*(n^2-1) which differ by n.
 * https://oeis.org/A299068
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of divisors of n^2*(n^2-1) that differ by
 *            n
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 562, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void add_prime(uint64_t p, unsigned int e,
                      uint64_t prime_list[], unsigned int exp_list[], size_t *cnt)
{
    for (size_t i = 0; i < *cnt; ++i)
        if (prime_list[i] == p) {
            exp_list[i] += e;
            return;
        }
    prime_list[*cnt] = p;
    exp_list[*cnt] = e;
    ++(*cnt);
}

static void enumerate_divisors(size_t idx, seq_t cur,
                               const uint64_t *primes, const unsigned int *exps,
                               size_t pcnt,
                               const seq_t N, const seq_t n_seq,
                               size_t *count)
{
    if (idx == pcnt) {
        seq_t e = cur + n_seq;
        if (N % e == 0)
            ++(*count);
        return;
    }
    seq_t val = cur;
    for (unsigned int e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(idx + 1, val, primes, exps, pcnt, N, n_seq, count);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t N = n2 * (n2 - (seq_t)1);

    uint64_t primes[64];
    unsigned int exps[64];
    size_t pcnt = 0;

    uint64_t pr[64], ex[64];
    size_t cnt;

    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        add_prime(pr[i], (unsigned int)(ex[i] * 2), primes, exps, &pcnt);

    if (n > 1) {
        factor_u64(n - 1, pr, ex, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            add_prime(pr[i], (unsigned int)ex[i], primes, exps, &pcnt);
    }

    factor_u64(n + 1, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        add_prime(pr[i], (unsigned int)ex[i], primes, exps, &pcnt);

    size_t count = 0;
    enumerate_divisors(0, (seq_t)1, primes, exps, pcnt, N, n_seq, &count);
    return (seq_t)count;
}
