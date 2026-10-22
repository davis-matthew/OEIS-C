/*
 * A337162  Numbers m such that Sum_{d|m: 1<=d<=sqrt(m)} m/d-d is a multiple
 *          of m.
 * https://oeis.org/A337162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   A337162 is the sequence of positive integers m such that
 *            Sum_{d|m, 1 <= d <= sqrt(m)} (m/d - d) is a multiple of m
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --- Integer square root -------------------------------------------- */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;  /* start with highest power of 4 <= 2^64 */
    while (bit <= x) bit <<= 2;
    while (bit > 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* --- Prime generation ----------------------------------------------- */
static uint32_t *prime_list = NULL;
static size_t     prime_cnt  = 0;
static uint64_t   prime_limit = 0;

static void prune_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;

    size_t n = (size_t)limit + 1;
    char *is_prime = (char*)malloc(n);
    if (!is_prime) exit(1);
    memset(is_prime, 1, n);
    if (n > 0) is_prime[0] = 0;
    if (n > 1) is_prime[1] = 0;

    for (uint64_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (uint64_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    free(is_prime);
    prime_cnt  = cnt;
    prime_limit = limit;
}

/* --- Factorisation ----------------------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *pf, int *exp, size_t *pf_cnt)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[cnt]   = (uint64_t)p;
            exp[cnt]  = e;
            ++cnt;
        }
    }
    if (n > 1) {
        pf[cnt]   = n;
        exp[cnt]  = 1;
        ++cnt;
    }
    *pf_cnt = cnt;
}

/* --- Recursively iterate small divisors --------------------------- */
static void dfs_divs(size_t idx,
                     uint64_t cur,
                     const uint64_t *pf,
                     const int      *exp,
                     size_t          pf_cnt,
                     uint64_t        sqrt_m,
                     uint64_t        m,
                     uint64_t       *sum_mod)
{
    if (cur > sqrt_m) return;            /* pruning */

    if (idx == pf_cnt) {
        /* cur is a divisor <= sqrt(m) */
        uint64_t term = m / cur;          /* >= cur for d <= sqrt(m) */
        uint64_t diff = term - cur;       /* in [0, m) */
        __uint128_t s = (__uint128_t)*sum_mod + diff;
        if (s >= m) s -= m;
        *sum_mod = (uint64_t)s;
        return;
    }

    uint64_t p = pf[idx];
    uint64_t next = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        dfs_divs(idx + 1, next, pf, exp, pf_cnt, sqrt_m, m, sum_mod);
        if (e == exp[idx]) break;
        next *= p;
    }
}

/* --- Sequence generator ------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t  processed = 0;    /* last m inspected */

    if (n == 0) return 0;          /* not used, offset starts at 1 */

    while (cache_len < (size_t)n) {
        ++processed;                /* next m to test */
        uint64_t m       = (uint64_t)processed;
        uint64_t sqrt_m  = isqrt_u64(m);

        prune_primes(sqrt_m);

        /* factorise m */
        uint64_t primes[64];
        int      exps[64];
        size_t   pf_cnt;
        factor_uint64(m, primes, exps, &pf_cnt);

        /* compute S(m) modulo m */
        uint64_t sum_mod = 0;
        dfs_divs(0, 1, primes, exps, pf_cnt, sqrt_m, m, &sum_mod);

        if (sum_mod == 0) {
            /* store term */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
