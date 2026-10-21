/*
 * A279314  Composite numbers n such that the sum of the prime factors of n,
 *          with multiplicity, is congruent to n (mod 9).
 * https://oeis.org/A279314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Composite numbers n such that the sum of the prime factors of n
 *            * (with multiplicity) is congruent to n modulo 9. * The function
 *            returns the n‑th term of that sequence (n ≥ 1)
 * Range:     u8 n <= 15, u16 n <= 4677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime generation ---------- */
#define PRIME_SIEVE_LIMIT 2000000   /* covers sqrt of the largest needed n */

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static bool   primes_ready = false;

static void build_primes(void)
{
    if (primes_ready) return;
    const uint64_t limit = PRIME_SIEVE_LIMIT;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    /* count primes to allocate an array of the exact size */
    size_t pcount = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++pcount;
    prime_list = (uint64_t *)malloc(pcount * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) prime_list[idx++] = i;
    prime_cnt = pcount;
    free(is_prime);
    primes_ready = true;
}

/* factorization using pre‑computed primes */
static size_t factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[cnt] = p;
            ex[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        pr[cnt] = tmp;
        ex[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* ---------- cache for sequence values ---------- */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
static uint64_t next_candidate = 4;   /* first composite > 1 */

static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* main function returning the n‑th term */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe to return 0 */

    build_primes();   /* ensure prime table is ready */

    while (cache_sz < (size_t)n) {
        uint64_t cand = next_candidate;

        /* factor cand */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t cnt = factor_uint64(cand, pr, ex);

        /* skip primes */
        if (cnt == 1 && ex[0] == 1) {
            ++next_candidate;
            continue;
        }

        /* sum of prime factors with multiplicity */
        uint64_t sum = 0;
        for (size_t i = 0; i < cnt; ++i) sum += pr[i] * ex[i];

        /* check congruence modulo 9 */
        if (((cand - sum) % 9) == 0) {
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = (seq_t)cand;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
