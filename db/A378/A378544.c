/*
 * A378544  a(n) is the sum of those divisors d of n for which A083345(d) is
 *          even, where A083345(n) is the numerator of Sum(e/p:
 *          n=Product(p^e)).
 * https://oeis.org/A378544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, A083345(d) even} d
 * Range:     u8 n <= 224, u16 n <= 9258, u32 n <= 9258, u64 n <= 9258,
 *            u128 n <= 9258, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, a(n) = Sum_{d|n} d * A369001(d)) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd64.h"

/* Smallest prime factor array, dynamic up to current maximum. */
static uint64_t *spf = NULL;
static size_t   spf_cap = 0;

/* Parity cache: 1 if A083345(d) is even, 0 otherwise. */
static uint8_t *parity_cache = NULL;
static size_t   parity_cap = 0;

/* Ensure spf is built up to at least 'limit'. */
static void sieve_spf(idx_t limit)
{
    if ((size_t)limit + 1 <= spf_cap)
        return;

    size_t new_cap = (size_t)limit + 1;
    uint64_t *new_spf = (uint64_t *)realloc(spf, new_cap * sizeof(uint64_t));
    if (!new_spf)
        exit(1);
    spf = new_spf;

    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = i;
    spf[0] = 0;
    spf[1] = 1;

    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
    spf_cap = new_cap;
}

/* Euclidean gcd for 64‑bit integers. */

/* Determine if A083345(x) has even numerator. */
static bool parity_of(uint64_t x)
{
    if (x == 1)
        return true;   /* numerator 0 is even */

    uint64_t tmp = x;
    uint64_t primes[10];
    uint64_t exps[10];
    size_t k = 0;

    while (tmp > 1) {
        uint64_t p = spf[tmp];
        uint64_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[k] = p;
        exps[k]   = e;
        ++k;
    }

    uint64_t D = 1;
    for (size_t i = 0; i < k; ++i)
        D *= primes[i];

    uint64_t N = 0;
    for (size_t i = 0; i < k; ++i) {
        uint64_t M = D / primes[i];
        N += exps[i] * M;
    }

    uint64_t g = gcd64(N, D);
    N /= g;
    return (N & 1ULL) == 0ULL;
}

/* Extend parity_cache to include all integers up to n. */
static void update_parity_cache_to(idx_t n)
{
    sieve_spf(n);

    size_t needed = (size_t)n + 1;
    if (needed > parity_cap) {
        uint8_t *new_cache = (uint8_t *)realloc(parity_cache, needed * sizeof(uint8_t));
        if (!new_cache)
            exit(1);
        parity_cache = new_cache;

        for (idx_t i = parity_cap; i <= n; ++i)
            parity_cache[i] = parity_of((uint64_t)i) ? 1U : 0U;

        parity_cap = needed;
    }
}

/* Data for recursive divisor construction. */
static uint64_t cur_primes[10];
static uint64_t cur_exps[10];
static size_t   cur_k;
static seq_t    cur_sum;

/* Recursive divisor generation: at position idx with current product cur. */
static void gen_divs_rec(size_t idx, uint64_t cur)
{
    if (idx == cur_k) {
        if (parity_cache[cur])
            cur_sum += (seq_t)cur;
        return;
    }
    uint64_t p = cur_primes[idx];
    uint64_t val = cur;
    for (size_t e = 0; e <= cur_exps[idx]; ++e) {
        gen_divs_rec(idx + 1, val);
        val *= p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    update_parity_cache_to(n);

    /* Factor n. */
    uint64_t m = n;
    uint64_t primes[10];
    uint64_t exps[10];
    size_t k = 0;
    while (m > 1) {
        uint64_t p = spf[m];
        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        primes[k] = p;
        exps[k]   = e;
        ++k;
    }

    /* Prepare for divisor enumeration. */
    cur_k = k;
    for (size_t i = 0; i < k; ++i) {
        cur_primes[i] = primes[i];
        cur_exps[i]   = exps[i];
    }
    cur_sum = (seq_t)0;

    gen_divs_rec(0, 1ULL);

    return cur_sum;
}
