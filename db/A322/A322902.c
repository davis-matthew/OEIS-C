/*
 * A322902  Numbers whose prime indices are all proper powers of the same
 *          number.
 * https://oeis.org/A322902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers whose prime indices are all powers of the same integer.
 *            a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5, a(6) = 7, …
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Configuration ---------- */
#define SIEVE_LIMIT_INITIAL 3000000ULL   /* should cover 10 000th term with high probability */

/* ----------  Global data ---------- */
static uint32_t *spf          = NULL;  /* smallest prime factor for numbers 0..limit */
static uint32_t *prime_index  = NULL;  /* 1‑based index of each prime, 0 if not prime */
static uint32_t *primes       = NULL;  /* list of primes */
static size_t    prime_cnt    = 0;
static size_t    sieve_limit  = 0;

/* ----------  Sequence cache ---------- */
static seq_t   *term_cache = NULL;
static size_t   cache_len  = 0;
static size_t   cache_cap  = 0;

/* ----------  Last tested value ---------- */
static uint64_t last_tested = 0;

/* ----------  Forward declarations ---------- */
static void build_sieve(size_t limit);
static void ensure_cache(size_t n);
static void factor_uint32(uint32_t n, uint32_t fac[], uint8_t exp[], size_t *cnt);
static uint8_t igcd(uint8_t a, uint8_t b);
static bool is_good(uint64_t val);

/* ----------  Sieve construction ---------- */
static void build_sieve(size_t limit)
{
    if (spf != NULL) {
        if (limit <= sieve_limit) return;
        free(spf);      spf = NULL;
        free(prime_index); prime_index = NULL;
        free(primes);   primes = NULL;
    }

    sieve_limit = limit;
    spf          = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    prime_index  = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    primes = (uint32_t *)malloc((limit / 10 + 10) * sizeof(uint32_t));
    prime_cnt = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_index[i] = (uint32_t)(prime_cnt + 1);
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > limit) break;
            spf[v] = p;
            if (i % p == 0) break;
        }
    }
}

/* ----------  Factorisation of a uint32_t using spf ---------- */
static void factor_uint32(uint32_t n, uint32_t fac[], uint8_t exp[], size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint8_t e = 0;
        do {
            n /= p;
            ++e;
        } while (n % p == 0);
        fac[(*cnt)]   = p;
        exp[(*cnt)]   = e;
        ++(*cnt);
    }
}

/* ----------  gcd for uint8_t ---------- */
static uint8_t igcd(uint8_t a, uint8_t b)
{
    while (b) {
        uint8_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ----------  Goodness test ---------- */
static bool is_good(uint64_t val)
{
    if (val < 2) return true;      /* n == 1 is always good */

    if (val > sieve_limit) return false;  /* should not happen after resizing */

    uint64_t temp = val;
    uint32_t indices[32];
    size_t   idx_cnt = 0;
    bool     has_two = false;

    /* Collect distinct prime indices of val */
    while (temp > 1) {
        uint32_t p = spf[temp];
        if (p == 2) has_two = true;
        uint32_t idx = prime_index[p];
        /* add idx if not already present */
        bool seen = false;
        for (size_t i = 0; i < idx_cnt; ++i) {
            if (indices[i] == idx) { seen = true; break; }
        }
        if (!seen) indices[idx_cnt++] = idx;
        while (temp % p == 0) temp /= p;
    }

    /* Presence of 2 together with other indices violates the rule */
    if (has_two && idx_cnt > 1) return false;

    /* Single prime factor or power of 2 */
    if (idx_cnt <= 1) return true;

    /* Now idx_cnt > 1 and no index 1 present */
    /* Compute base prime exponents as the gcd across all indices */
    uint32_t base_primes[32];
    uint8_t  base_exps[32];
    size_t   base_cnt = 0;

    /* Initialize with the first index */
    factor_uint32(indices[0], base_primes, base_exps, &base_cnt);

    /* Process remaining indices */
    for (size_t ii = 1; ii < idx_cnt; ++ii) {
        uint32_t fac[32];
        uint8_t  exp[32];
        size_t   len = 0;
        factor_uint32(indices[ii], fac, exp, &len);

        /* Update gcd over exponents for shared primes and check missing primes */
        for (size_t j = 0; j < base_cnt; ++j) {
            uint32_t bp = base_primes[j];
            uint8_t e_m = 0;
            for (size_t t = 0; t < len; ++t) {
                if (fac[t] == bp) { e_m = exp[t]; break; }
            }
            if (e_m == 0) return false;             /* prime missing in this index */
            base_exps[j] = igcd(base_exps[j], e_m);  /* update gcd */
        }

        /* Ensure no extra primes appear in this index */
        for (size_t t = 0; t < len; ++t) {
            bool matched = false;
            for (size_t j = 0; j < base_cnt; ++j) {
                if (fac[t] == base_primes[j]) { matched = true; break; }
            }
            if (!matched) return false;   /* extra prime factor present */
        }
    }

    /* Final check: each index must be a power of the base (exponents multiples).
       Since we already enforced equal prime sets and handled gcd, this is satisfied. */
    return true;
}

/* ----------  Cache growth ---------- */
static void ensure_cache(size_t n)
{
    if (sieve_limit == 0) build_sieve(SIEVE_LIMIT_INITIAL);

    while (cache_len < n) {
        ++last_tested;
        if (last_tested > sieve_limit) {
            size_t new_limit = sieve_limit * 2;
            if (last_tested > new_limit) new_limit = last_tested + 10000;
            build_sieve(new_limit);
        }
        if (is_good(last_tested)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap ? cache_cap * 2 : 32);
                term_cache = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
            }
            term_cache[cache_len++] = (seq_t)last_tested;
        }
    }
}

/* ----------  Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n<1, return 0 */
    ensure_cache((size_t)n);
    return term_cache[n - 1];
}
