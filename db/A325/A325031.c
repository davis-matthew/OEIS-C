/*
 * A325031  Numbers divisible by all prime indices of their prime indices.
 * https://oeis.org/A325031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A325031: numbers divisible by all prime indices of their prime indices. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

#define INITIAL_PRIME_LIMIT 2000000ULL   /* primes used for index look‑ups */
#define INITIAL_CACHE_CAP   128           /* initial cache size */

/* ---------- Prime list -------------------------------------------------- */
static uint64_t *prime_list = NULL; /* array of primes                     */
static size_t   prime_cnt   = 0;     /* number of primes in the list        */

/* Build the prime list once.                                 */
static void build_prime_list(void)
{
    if (prime_list != NULL)
        return;
    size_t cnt;
    build_primes_upto(INITIAL_PRIME_LIMIT, &prime_list, &cnt);
    prime_cnt = cnt;
}

/* Return the 0‑based index of a prime `p`.  If `p` is not found
   the size of the list is returned – should not happen for primes. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return mid;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return prime_cnt; /* not found */
}

/* ---------- Cache for the sequence ------------------------------------- */
static seq_t *cache      = NULL;   /* cache of a(1..n) */
static size_t cache_cap  = 0;      /* cache capacity   */
static size_t cache_len  = 0;      /* number of elements cached */
static uint64_t next_candidate = 1;/* next integer to test */

/* Grow the cache to at least `needed` elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : INITIAL_CACHE_CAP;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ---------- Test whether x satisfies the definition ------------------- */
static bool is_good(uint64_t x)
{
    if (x == 1)
        return true;                /* 1 has no prime divisors – vacuously true */

    uint64_t p[64], e[64];
    size_t pcnt;
    factor_u64(x, p, e, &pcnt);

    /* For every prime divisor `p` of `x` */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t prime = p[i];
        size_t idx0 = prime_index(prime);            /* 0‑based index */
        if (idx0 == prime_cnt)                       /* safety check */
            return false;
        uint64_t m = (uint64_t)idx0 + 1;             /* 1‑based index */

        /* Prime factorise `m` (the index of the prime divisor).      */
        uint64_t fac[32], exp[32];
        size_t icnt;
        factor_u64(m, fac, exp, &icnt);

        /* Every prime factor `q` of `m` yields an index `r`.  The
           condition requires that `x` be divisible by `r`.  (Since `r`
           is a 1‑based index, convert it to a seq_t.)              */
        for (size_t j = 0; j < icnt; ++j) {
            uint64_t q = fac[j];
            size_t idxq0 = prime_index(q);
            if (idxq0 == prime_cnt)
                return false;
            seq_t r = (seq_t)(idxq0 + 1);   /* 1‑based index of q */
            if (((seq_t)x) % r != 0)
                return false;
        }
    }
    return true;
}

/* ---------- Main sequence routine ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined below the offset */

    build_prime_list();

    /* Ensure all terms up to `n` are computed. */
    size_t target = (size_t)n;
    if (cache_len < target) {
        ensure_cache(target);
        while (cache_len < target) {
            if (is_good(next_candidate))
                cache[cache_len++] = (seq_t)next_candidate;
            ++next_candidate;
        }
    }
    return cache[target - 1];
}
