/*
 * A317545  Number of multimin factorizations of n.
 * https://oeis.org/A317545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1, a(n) = Σ_{d | (n / p)} a(d), where p is the
 *            smallest prime divisor of n
 * Range:     u8 n <= 383, u16 n <= 9342, u32 n <= 9342, u64 n <= 9342,
 *            u128 n <= 9342, big any n
 * Helpers:   alloc_cache, build_spf, ensure_cache, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External functions and types provided by the build */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/alloc_cache.h"      /* seq_t *alloc_cache(idx_t n); */
#include "../utils/ensure_cache.h"     /* void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr); */
#include "../utils/build_spf.h"        /* void build_spf(idx_t limit, idx_t **spf_out); */
#include "../utils/alloc_cache.h"      /* seq_t *alloc_cache(idx_t n); */

static seq_t *cache = NULL;      /* cache[n] holds a(n); 0 means uninitialised */
static size_t cache_cap = 0;

static idx_t *spf = NULL;        /* smallest prime factor array */
static size_t spf_cap = 0;

/* forward declaration for recursive routine */
static seq_t a_rec(idx_t n);

/* ensure the spf array covers at least n */
static void ensure_spf(idx_t n)
{
    if (spf == NULL || spf_cap < n) {
        if (spf)
            free(spf);
        build_spf(n, &spf);
        spf_cap = n;
    }
}

/* generate all divisors of m and accumulate a(d) values */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t idx, idx_t cur, seq_t *sum)
{
    if (idx == cnt) {
        *sum += a_rec(cur);
        return;
    }
    uint64_t p = pr[idx];
    idx_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(pr, ex, cnt, idx + 1, val, sum);
        val *= p;
    }
}

/* recursive definition of a(n) */
static seq_t a_rec(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (cache[n] != 0)
        return cache[n];

    /* smallest prime factor */
    uint64_t p = (uint64_t)spf[n];
    idx_t m = n / p;

    /* factor m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    seq_t sum = 0;
    gen_divisors(primes, exps, cnt, 0, 1, &sum);

    cache[n] = sum;
    return sum;
}

/* public API required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined behaviour, but return 0 for safety */
        return 0;

    ensure_cache(n, &cache, &cache_cap);
    ensure_spf(n);

    return a_rec(n);
}
