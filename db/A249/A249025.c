/*
 * A249025  Numbers k such that 3^k - 1 is not squarefree.
 * https://oeis.org/A249025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 407, u32 n <= 407, u64 n <= 407,
 *            u128 n <= 407, big any n
 * Helpers:   mul_mod_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A249025 – numbers k such that 3^k – 1 is NOT squarefree.
   The algorithm marks all k that are multiples of the multiplicative
   order of 3 modulo p² for every prime p ≠ 3.  All even k are always
   marked, so the density of marked k is ≥ 1/2.  The cache is rebuilt
   as many times as necessary to satisfy any requested index. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mul_mod_uint.h"

static inline uint64_t powmod_uint(uint64_t base, uint64_t exp,
                                   uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            res = mul_mod_uint(res, base, mod);
        base = mul_mod_uint(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

/*------------- global cache --------------------------------------------*/
static seq_t *cache = NULL;      /* array of k values (ascending)          */
static size_t cache_len = 0;     /* how many values are stored           */

/*------------- rebuild cache up to a numeric limit --------------------*/
/* After this function cache_len holds the number of values stored in
   cache[0 .. cache_len-1] and all k ≤ need_limit that satisfy the
   condition are present in the array (though the array may contain
   values > need_limit if a later call asks for more).                   */
static void build_cache(idx_t need_limit)
{
    /* free any previous data */
    free(cache);
    cache = NULL;
    cache_len = 0;

    if (need_limit < 1) return;

    /* Step 1: sieve primes up to need_limit */
    void *raw = malloc((size_t)need_limit + 1);
    if (!raw) return;
    bool *is_prime = (bool*)raw;
    memset(is_prime, 1, (size_t)need_limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= need_limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= need_limit; j += i)
                is_prime[j] = 0;

    /* collect primes in array */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= need_limit; ++i)
        if (is_prime[i]) ++prime_cnt;

    uint64_t *primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    size_t pidx = 0;
    for (idx_t i = 2; i <= need_limit && pidx < prime_cnt; ++i)
        if (is_prime[i]) primes[pidx++] = (uint64_t)i;
    free(is_prime);

    /* Step 2: mark all k ≤ need_limit with a square factor */
    bool *bad = (bool*)calloc((size_t)need_limit + 1, 1);   /* 1 = 3^k-1 not squarefree */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p == 3) continue;              /* 3 never contributes a square factor */

        /* compute order d of 3 modulo p² */
        uint64_t d = p - 1;                /* p | p-1? yes, but p must divide p-1?? not used */
        /* factor p-1 using the same prime list (sufficient up to need_limit) */
        uint64_t tmp = d;
        for (size_t qi = 0; qi < prime_cnt && primes[qi] * primes[qi] <= tmp; ++qi) {
            uint64_t q = primes[qi];
            if (tmp % q == 0) {
                while (tmp % q == 0) tmp /= q;
                while (d % q == 0 && powmod_uint(3, d / q, p) == 1ULL)
                    d /= q;
            }
        }
        if (tmp > 1) {
            while (d % tmp == 0 && powmod_uint(3, d / tmp, p) == 1ULL)
                d /= tmp;
        }

        /* adjust to order modulo p² */
        uint64_t p2 = p * p;
        if (powmod_uint(3, d, p2) != 1ULL)
            d *= p;                       /* multiply by p if needed */

        if (d == 0ULL || d > (uint64_t)need_limit) continue;

        /* mark all multiples of d */
        for (uint64_t k = d; k <= (uint64_t)need_limit; k += d)
            bad[(size_t)k] = 1;
    }

    /* Step 3: build array of marked k */
    size_t cnt = 0;
    for (idx_t k = 1; k <= need_limit; ++k)
        if (bad[(size_t)k]) ++cnt;

    cache = (seq_t*)malloc(cnt * sizeof(seq_t));
    size_t pos = 0;
    for (idx_t k = 1; k <= need_limit; ++k)
        if (bad[(size_t)k]) cache[pos++] = (seq_t)k;

    cache_len = cnt;
    free(bad);
    free(primes);
}

/*--------------------------- public function --------------------------*/
seq_t a(idx_t n)
{
    /* ensure that we have at least n terms cached */
    if (cache_len < (size_t)n) {
        idx_t limit = n * 2ULL;
        if (limit < 1000ULL) limit = 1000ULL;
        build_cache(limit);
        while (cache_len < (size_t)n) {
            limit *= 2;
            build_cache(limit);
        }
    }
    return cache[(size_t)(n - 1)];
}
