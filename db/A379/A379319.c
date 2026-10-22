/*
 * A379319  Even numbers whose product of prime indices is a multiple of their
 *          sum of prime indices.
 * https://oeis.org/A379319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A379319: Even numbers whose product of prime indices is a multiple of their sum of prime indices. */
/*  a(1)=2, a(2)=30, a(3)=84, …   (offset = 1)  */
/*  No simple closed formula; we generate terms on demand by sieving primes and testing each even integer. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/floor_isqrt_u64.h"   /* returns idx_t floor(sqrt(n)) */
#include "../utils/prime_pi.h"          /* seq_t prime_pi(idx_t limit) */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) */

/* -------------------------------------------------------------------- */
/*  Prime table (kept up to a maximum value).                           */
/* -------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static idx_t    prime_max  = 0;

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_max) return;

    uint64_t *pr = NULL;
    size_t   cnt = 0;
    build_primes_upto(limit, &pr, &cnt);
    if (pr == NULL) return;        /* insufficient memory – abort silently */

    free(prime_list);
    prime_list = pr;
    prime_cnt  = cnt;
    prime_max  = limit;
}

/* -------------------------------------------------------------------- */
/*  Cache of sequence terms.  We store terms in order of increasing */
/*  index a(1), a(2), … .  The cache grows lazily as needed.          */
/* -------------------------------------------------------------------- */
static seq_t *term_cache    = NULL;
static size_t cache_len     = 0;      /* number of terms stored (>=1) */
static size_t cache_cap     = 0;      /* allocated capacity */
static idx_t  next_candidate = 2;     /* next even integer to test */

static void cache_grow(size_t need)
{
    if (cache_cap > need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    term_cache = tmp;
    cache_cap  = new_cap;
}

/* -------------------------------------------------------------------- */
/*  Test whether an even integer n satisfies the defining property.    */
/* -------------------------------------------------------------------- */
static bool satisfies(idx_t n)
{
    if ((n & 1u) != 0) return false;
    if (n <= 1) return false;

    idx_t temp = n;
    idx_t lim  = floor_isqrt_u64(temp) + 1;
    ensure_primes_upto(lim);

    unsigned long long sum = 0ULL;           /* sum of prime indices (with multiplicities) */
    __uint128_t prod = 1;                    /* product of prime indices (full precision) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > lim) break;
        if (temp % p != 0) continue;
        int expo = 0;
        while (temp % p == 0) {
            temp /= p;
            ++expo;
        }
        unsigned long long idx = (unsigned long long)(i + 1);   /* prime index is i+1 */
        sum += idx * (unsigned long long)expo;
        for (int k = 0; k < expo; ++k) {
            prod = prod * idx;
        }
    }

    if (temp > 1) { /* remaining prime factor > sqrt(n) */
        seq_t idx_seq = prime_pi((idx_t)temp);         /* index of the remaining prime */
        unsigned long long idx = (unsigned long long)idx_seq;
        sum += idx;
        prod = prod * idx;
    }

    /* product modulo sum must be zero */
    if (sum == 0) return false;                       /* should not happen for even n>2 */
    return (prod % sum) == 0U;
}

/* -------------------------------------------------------------------- */
/*  Ensure that the cache has at least target+1 terms (0‑based).       */
/* -------------------------------------------------------------------- */
static void ensure_cache(idx_t target)
{
    if (cache_len > target) return;

    cache_grow(target);

    while (cache_len <= target) {
        while (!satisfies(next_candidate)) ++next_candidate; /* skip non‑solutions */
        term_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* -------------------------------------------------------------------- */
/*  Public function required by the framework.                           */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t target = n - 1;                /* zero‑based index in cache */
    ensure_cache(target);
    return term_cache[target];
}
