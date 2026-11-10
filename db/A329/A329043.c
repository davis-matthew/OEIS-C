/*
 * A329043  a(n) = Product_{d|A122111(n), d>1} A008578(1+A286561(n,d)), where
 *          A286561(n,d) gives the exponent of the highest power of d dividing
 *          n.
 * https://oeis.org/A329043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|A122111(n), d>1} A008578(1+A286561(n,d)). Only
 *            divisors d of gcd(n, A122111(n)) give a factor other than 1,
 *            because A008578(1) = 1. So the code finds gcd(n, A122111(n))
 *            from the prime factorization of A122111(n), without ever
 *            computing A122111(n) itself, which can overflow. For...
 * Range:     u8 n <= 55, u16 n <= 263, u32 n <= 1559, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8772 (2026-11-08)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

static uint64_t A329043_kth_prime(uint64_t k)
{
    uint64_t cnt = 0, c = 1;
    while (cnt < k) {
        ++c;
        bool ok = true;
        for (uint64_t q = 2; q * q <= c; ++q)
            if (c % q == 0) { ok = false; break; }
        if (ok) ++cnt;
    }
    return c;
}

static int A329043_cmp_u64(const void *x, const void *y)
{
    uint64_t va = *(const uint64_t *)x, vb = *(const uint64_t *)y;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Partition of prime indices (with multiplicity), sorted */
    uint64_t idx[128];
    size_t cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t k = (uint64_t)prime_pi((idx_t)primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) idx[cnt++] = k;
    }
    qsort(idx, cnt, sizeof(uint64_t), A329043_cmp_u64);

    /* Factorization of A122111(n): primes prime(remaining) ^ length */
    uint64_t cp[128], ce[128];
    size_t ccnt = 0;
    uint64_t remaining = (uint64_t)cnt, last_part = 1;
    size_t pos = 0;
    while (pos < cnt) {
        uint64_t cur = idx[pos];
        uint64_t freq = 0;
        while (pos < cnt && idx[pos] == cur) { ++freq; ++pos; }
        cp[ccnt] = A329043_kth_prime(remaining);
        ce[ccnt] = cur - last_part + 1;
        ++ccnt;
        remaining -= freq;
        last_part = cur + 1;
    }

    /* g = gcd(n, A122111(n)) */
    uint64_t g = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        for (size_t j = 0; j < ccnt; ++j) {
            if (cp[j] == primes[i]) {
                uint64_t m = exps[i] < ce[j] ? exps[i] : ce[j];
                for (uint64_t t = 0; t < m; ++t) g *= primes[i];
                break;
            }
        }
    }

    seq_t res = (seq_t)1;
    for (uint64_t d = 2; d <= g; ++d) {
        if (g % d != 0) continue;
        uint64_t t = (uint64_t)n, e = 0;
        while (t % d == 0) { t /= d; ++e; }
        res = res * (seq_t)A329043_kth_prime(e);
    }
    return res;
}
