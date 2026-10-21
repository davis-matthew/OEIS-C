/*
 * A254520  Möbius transform of A034676.
 * https://oeis.org/A254520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n^2 * Σ_{d^2 | n} μ(d)/d^2 Equivalently: multiplicative,
 *            a(p) = p^2, a(p^e) = p^(2e) – p^(2e‑2) for e>1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- integer square root for idx_t (unsigned 64‑bit) ---------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* ---------- integer power with small unsigned exponent ---------- */
static inline seq_t pow_seq_small(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp--)
        res *= base;
    return res;
}

/* ---------- prime sieve (cache up to current limit) ---------- */
static uint64_t *prime_cache = NULL;
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

static void ensure_prime_cache(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    free(prime_cache);
    prime_cache = NULL;
    prime_cnt   = 0;

    /* simple sieve of Eratosthenes up to 'limit' */
    if (limit < 2) {
        prime_limit = limit;
        return;
    }
    size_t sz = (size_t)limit + 1;
    /* boolean array: 0 = composite, 1 = prime candidate */
    char *mark = (char *)malloc(sz);
    if (!mark) exit(EXIT_FAILURE);
    for (size_t i = 0; i < sz; ++i)
        mark[i] = 1;             /* assume prime */
    mark[0] = mark[1] = 0;      /* 0 and 1 are not prime */

    for (size_t p = 2; p * p <= (size_t)limit; ++p) {
        if (mark[p]) {
            for (size_t j = p * p; j <= (size_t)limit; j += p)
                mark[j] = 0;
        }
    }

    /* count primes first to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (mark[i]) ++cnt;

    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (mark[i]) pr[idx++] = (uint64_t)i;

    free(mark);

    prime_cache = pr;
    prime_cnt   = cnt;
    prime_limit = limit;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* offset is 1, but guard against 0 */
    if (n == 1)
        return (seq_t)1;

    idx_t lim = isqrt_u64(n);
    ensure_prime_cache(lim);

    uint64_t m = (uint64_t)n;
    seq_t    res = 1;

    for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= m; ++i) {
        uint64_t p   = prime_cache[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e == 1) {
                /* a(p) = p^2 */
                res *= pow_seq_small((seq_t)p, 2);
            } else {
                seq_t p2e  = pow_seq_small((seq_t)p, 2 * e);
                seq_t p2e2 = pow_seq_small((seq_t)p, 2 * e - 2);
                res *= (p2e - p2e2);
            }
        }
    }

    /* remaining prime factor (exponent 1) */
    if (m > 1)
        res *= pow_seq_small((seq_t)m, 2);

    return res;
}
