/*
 * A347762  Number of compositions (ordered partitions) of n into at most 2
 *          prime powers (including 1).
 * https://oeis.org/A347762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n is a prime power (including 1)] + #{(p , q) : p + q =
 *            n , p , q are prime powers (including 1)}
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- cached data ---------- */
static idx_t  max_pp_limit = 0;          /* largest n for which prime powers were built */
static unsigned char *pow_flag = NULL;   /* pow_flag[v] == 1 if v is a prime power   */
static size_t  pow_flag_cap = 0;          /* allocated size of pow_flag              */

static idx_t *prime_powers = NULL;       /* list of all prime powers up to the limit */
static size_t pp_cnt   = 0, pp_cap    = 0;

static seq_t *cache        = NULL;       /* memoised sequence values                 */
static size_t cache_cap   = 0;           /* allocated size of cache                  */
static idx_t  cached_up_to = 0;          /* largest n that has been cached           */

/* ---------- build all prime powers (including 1) up to 'limit' ---------- */
static void build_prime_powers(idx_t limit)
{
    /* allocate or grow the flag array */
    if ((idx_t)(pow_flag_cap) <= limit) {
        unsigned char *new_flag = (unsigned char*)realloc(pow_flag,
                            (limit + 1) * sizeof(unsigned char));
        if (!new_flag) abort();
        pow_flag = new_flag;
        pow_flag_cap = limit + 1;
    }
    /* initialise flag array to 0 */
    for (idx_t i = 0; i <= limit; ++i) pow_flag[i] = 0;

    /* simple sieve for primes up to 'limit' */
    unsigned char *is_prime = (unsigned char*)malloc((limit + 1) * sizeof(unsigned char));
    if (!is_prime) abort();
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* allocate or grow the prime_powers array */
    if (pp_cap == 0) {
        pp_cap = 16;
        prime_powers = (idx_t*)malloc(pp_cap * sizeof(idx_t));
        if (!prime_powers) abort();
    }
    pp_cnt = 0;

    /* 1 is a prime power */
    if (pp_cnt == pp_cap) {
        pp_cap *= 2;
        prime_powers = (idx_t*)realloc(prime_powers, pp_cap * sizeof(idx_t));
        if (!prime_powers) abort();
    }
    prime_powers[pp_cnt++] = 1;
    pow_flag[1] = 1;

    /* generate prime powers for each prime */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        idx_t val = p;
        while (val <= limit) {
            if (pp_cnt == pp_cap) {
                pp_cap *= 2;
                prime_powers = (idx_t*)realloc(prime_powers, pp_cap * sizeof(idx_t));
                if (!prime_powers) abort();
            }
            prime_powers[pp_cnt++] = val;
            pow_flag[val] = 1;
            if (val > limit / p) break;   /* avoid overflow */
            val *= p;
        }
    }

    free(is_prime);
    max_pp_limit = limit;
}

/* ---------- ensure cache is large enough ---------- */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- main entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* build prime powers up to n (single part may equal n) */
    if (n > max_pp_limit) build_prime_powers(n);
    ensure_cache(n);

    /* compute missing values up to n */
    if (n > cached_up_to) {
        for (idx_t k = cached_up_to + 1; k <= n; ++k) {
            seq_t val = 0;
            /* single part */
            if (pow_flag[k]) val += 1;
            /* two parts */
            for (size_t idx = 0; idx < pp_cnt; ++idx) {
                idx_t p = prime_powers[idx];
                if (p > k - 1) continue;  /* q would be non‑positive */
                idx_t q = k - p;
                if (pow_flag[q]) val += 1;
            }
            cache[k] = val;
        }
        cached_up_to = n;
    }
    return cache[n];
}
