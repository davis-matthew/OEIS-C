/*
 * A353378  Number of ways to write the square of n as a product of the terms
 *          of A345452 larger than 1; a(1) = 1 by convention (an empty
 *          product).
 * https://oeis.org/A353378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to factor n^2 into factors k>1 such that k
 *            has an even number of prime factors (counted with multiplicity)
 *            and an even 2-adic valuation. The computation is performed by
 *            recursion with memoisation, enumerating all divisors of each
 *            intermediate product
 * Range:     u8 n <= 1259, u16 n <= 9740, u32 n <= 9740, u64 n <= 9740,
 *            u128 n <= 9740, big any n
 * Helpers:   make_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/make_key.h"

/* ---------- helper comparison prototype ----------
   Used by qsort to order divisors. ---------- */
static int compare_uint64(const void *a, const void *b);

/* ---------- prime setup ----------
   Generating all primes <= MAX_PRIME (≈ 10 000) once. ---------- */
#define MAX_PRIME 10000
static uint32_t primes[MAX_PRIME / 4 + 1];  /* enough for all primes <= 10000 */
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0)
        return;                /* already initialised */

    static bool sieve[MAX_PRIME + 1];
    for (size_t i = 2; i <= MAX_PRIME; ++i)
        if (!sieve[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= MAX_PRIME)
                for (size_t j = i * i; j <= MAX_PRIME; j += i)
                    sieve[j] = true;
        }
}

/* ---------- test whether a divisor is allowed (A345452) ----------
   Conditions:
     * k > 1
     * 2-adic valuation of k is even
     * total number of prime factors (with multiplicity) is even
   ---------- */
static bool is_allowed(uint64_t k)
{
    if (k <= 1U)
        return false;          /* must be > 1 */

    /* even 2-adic valuation */
    unsigned v2 = 0;
    while ((k & 1U) == 0U) {
        k >>= 1U;
        ++v2;
    }
    if (v2 & 1U)
        return false;

    /* even total number of prime factors */
    unsigned odd_cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= k; ++i) {
        uint32_t p = primes[i];
        unsigned e = 0;
        while (k % p == 0U) {
            k /= p;
            ++e;
        }
        if (e & 1U)
            ++odd_cnt;
    }
    if (k > 1U)                 /* remaining prime factor */
        ++odd_cnt;

    return (odd_cnt & 1U) == 0U;
}

/* ---------- hash table for memoisation ----------
   Key: (m << 27) | min_factor   (both < 2^27 for m <= 10^8)
   ---------- */
#define HASH_SIZE (1 << 20)  /* 1 048 576 entries */
typedef struct {
    uint64_t key;   /* 0 indicates empty */
    seq_t    value;
} CacheEntry;
static CacheEntry cache[HASH_SIZE];
static bool cache_inited = false;

static seq_t cache_get(uint64_t key, bool *found)
{
    if (!cache_inited) {
        memset(cache, 0, sizeof(cache));
        cache_inited = true;
    }
    uint64_t h = key & (HASH_SIZE - 1);
    while (1) {
        if (cache[h].key == 0) {
            *found = false;
            return 0;
        }
        if (cache[h].key == key) {
            *found = true;
            return cache[h].value;
        }
        h = (h + 1) & (HASH_SIZE - 1);
    }
}

static void cache_put(uint64_t key, seq_t val)
{
    uint64_t h = key & (HASH_SIZE - 1);
    while (1) {
        if (cache[h].key == 0) {
            cache[h].key = key;
            cache[h].value = val;
            return;
        }
        if (cache[h].key == key) {
            cache[h].value = val;
            return;
        }
        h = (h + 1) & (HASH_SIZE - 1);
    }
}

/* ---------- factorisation utilities ----------
   Factor n into primes using precomputed primes. ---------- */
static void factor_uint64(uint64_t n, uint64_t *pf, int *exp, size_t *cnt)
{
    *cnt = 0;
    uint64_t x = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p = primes[i];
        if (x % p == 0U) {
            int e = 0;
            while (x % p == 0U) {
                x /= p;
                ++e;
            }
            pf[*cnt] = p;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (x > 1U) {
        pf[*cnt] = x;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* ---------- generate all divisors of a number ----------
   Recursively build divisors from prime factors. ---------- */
static void gen_divisors_rec(const uint64_t *pr, const int *ex, size_t idx,
                             uint64_t cur, uint64_t *arr, size_t *cnt)
{
    if (idx == 0) {
        arr[(*cnt)++] = cur;
        return;
    }
    /* recurse without current prime factor */
    gen_divisors_rec(pr, ex, idx - 1, cur, arr, cnt);
    /* recurse with current prime factor at various exponents */
    uint64_t base = 1;
    for (int e = 1; e <= ex[idx - 1]; ++e) {
        base *= pr[idx - 1];
        gen_divisors_rec(pr, ex, idx - 1, cur * base, arr, cnt);
    }
}

/* ---------- recursive counting of factorizations ----------
   m: number to be factored
   minf: minimal factor allowed (to avoid duplicate permutations) ---------- */
static seq_t dfs(uint64_t m, uint64_t minf)
{
    if (m == 1ULL)
        return (seq_t)1;      /* completed a factorisation */

    bool found;
    uint64_t key = make_key(m, minf);
    seq_t val = cache_get(key, &found);
    if (found)
        return val;

    /* factor m into primes */
    uint64_t pf[64];
    int exp[64];
    size_t cnt_pf;
    factor_uint64(m, pf, exp, &cnt_pf);

    /* generate all divisors of m */
    uint64_t divs[4096];
    size_t div_cnt = 0;
    gen_divisors_rec(pf, exp, cnt_pf, 1ULL, divs, &div_cnt);

    /* sort divisors ascending to enforce minf order */
    qsort(divs, div_cnt, sizeof(uint64_t),
          (int (*)(const void *, const void *))compare_uint64);

    seq_t total = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d < minf || d == 1ULL)
            continue;
        if (!is_allowed(d))
            continue;
        total += dfs(m / d, d);
    }

    cache_put(key, total);
    return total;
}

/* ---------- qsort comparator for uint64_t ----------
   Required for sorting divisors. ---------- */
static int compare_uint64(const void *a, const void *b)
{
    uint64_t aa = *(const uint64_t *)a;
    uint64_t bb = *(const uint64_t *)b;
    if (aa < bb)
        return -1;
    if (aa > bb)
        return 1;
    return 0;
}

/* ---------- main sequence function ----------
   Called by the OEIS harness. ---------- */
seq_t a(idx_t n)
{
    init_primes();                     /* ensure prime table ready */
    uint64_t m = (uint64_t)n * (uint64_t)n;  /* n^2 */
    return dfs(m, 2ULL);               /* factors must be >= 2 */
}
