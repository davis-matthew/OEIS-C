/*
 * A373903  Numbers k such that k^2 - 1 has fewer divisors than k^2 + 1.
 * https://oeis.org/A373903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2927, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373903: numbers k such that k^2-1 has fewer divisors
   than k^2+1.  The routine generates k sequentially, counts the
   divisors of k^2±1 by trial division using a short prime table,
   stores the qualifying k's, and returns the *n*‑th one.  All
   intermediate arithmetic is limited to +,-,*,/,% and bitwise
   operators; no floating point is used.  The algorithm is
   deterministic and properly memoises results between calls. */

/* ---------- common includes ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ---------- prime table ---------- */
#define PRIME_LIMIT 2000000  /* should cover k needed for n≈10000 */

#ifdef __cplusplus
/* C++: use std::vector for the prime list */
static std::vector<uint32_t> primes;
static bool primes_ready = false;

static void ensure_primes(void)
{
    if (primes_ready) return;
    std::vector<char> is_comp(PRIME_LIMIT + 1, 0);
    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) primes.push_back((uint32_t)i);
    primes_ready = true;
}
#else
/* C: static arrays for primes */
static const uint32_t *primes = NULL;
static size_t primes_count = 0;

static void ensure_primes(void)
{
    if (primes != NULL) return;
    bool *is_comp = (bool*)malloc(PRIME_LIMIT + 1);
    if (!is_comp) abort();
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_comp[i] = false;
    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
    /* count primes */
    size_t cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) ++cnt;
    uint32_t *arr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!arr) abort();
    size_t idx = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) arr[idx++] = (uint32_t)i;
    free(is_comp);
    primes = arr;
    primes_count = cnt;
}
#endif

/* ---------- divisor counting ---------- */
static uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t tmp = n;
    ensure_primes();

#ifdef __cplusplus
    for (size_t i = 0; i < primes.size() && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
#else
    for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
#endif

    if (tmp > 1) res *= 2;   /* remaining prime factor */
    return res;
}

/* ---------- caching of sequence terms ---------- */
static uint64_t next_k = 2;   /* next candidate k */

#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
#endif

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 */

    ensure_primes();

    /* generate terms until we have at least n of them */
    while (
#ifdef __cplusplus
        cache.size() < (size_t)n
#else
        cache_len < (size_t)n
#endif
    ) {
        uint64_t k = next_k++;
        __int128 sq128 = (__int128)k * k;
        if (sq128 > UINT64_MAX) {       /* overflow: stop generation */
            break;
        }
        uint64_t sq = (uint64_t)sq128;
        uint64_t v1 = sq - 1;
        uint64_t v2 = sq + 1;

        uint64_t d1 = divisor_count_uint64(v1);
        uint64_t d2 = divisor_count_uint64(v2);

        if (d1 < d2) {
#ifdef __cplusplus
            cache.push_back((seq_t)k);
#else
            /* grow array if needed */
            if (cache_cap == 0 || cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) abort();
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
#endif
        }
    }

#ifdef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
