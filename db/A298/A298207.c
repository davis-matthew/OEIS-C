/*
 * A298207  Numbers that are a product of zero, one, or three (not necessarily
 *          distinct) prime numbers.
 * https://oeis.org/A298207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298207: a(n) is the n-th positive integer whose
 * total number of prime factors (with multiplicity) is in {0, 1, 3}.
 * The implementation uses a sieve of smallest prime factors up to a
 * fixed limit and caches the generated terms.  The cache allows
 * arbitrary ordering of calls.  The code is valid C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* ---------- constants and globals ---------- */
static const size_t SIEVE_LIMIT = 2000000U;    /* sufficient for n ≤ 10 000 */

static bool sieve_ready = false;
static uint32_t *spf = NULL;          /* smallest prime factor */
static uint8_t  *bigomega = NULL;     /* total number of prime factors */

static bool cache_ready = false;
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;
static uint32_t next_value = 1;       /* next integer to test */

/* ---------- sieve construction ---------- */
static void init_sieve(void)
{
    if (sieve_ready) return;

    /* allocate arrays */
    spf = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    bigomega = (uint8_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    if (!spf || !bigomega) return;   /* allocation error */

    /* linear sieve for spf */
    uint32_t *primes = (uint32_t*)malloc((SIEVE_LIMIT / 10) * sizeof(uint32_t));
    size_t primes_cnt = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t v = (uint64_t)i * primes[j];
            if (v > SIEVE_LIMIT) break;
            spf[v] = primes[j];
            if (i % primes[j] == 0) break;
        }
    }
    free(primes);

    /* compute bigomega */
    bigomega[0] = 0;
    bigomega[1] = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        bigomega[i] = bigomega[i / spf[i]] + 1;
    }

    sieve_ready = true;
}

/* ---------- cache management ---------- */
static void ensure_cache(idx_t n)
{
    if (!cache_ready) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_cnt = 0;
        cache_ready = true;
    }
    if (cache_cnt >= (size_t)n) return;

    while (cache_cnt < (size_t)n) {
        if (next_value > SIEVE_LIMIT) {
            /* The chosen limit should cover all requested terms. */
            return;
        }
        uint8_t omega = bigomega[next_value];
        if (omega == 0 || omega == 1 || omega == 3) {
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
                memcpy(new_arr, cache, cache_cnt * sizeof(seq_t));
                free(cache);
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)next_value;
        }
        ++next_value;
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined at index 0, but safe */
    init_sieve();
    ensure_cache(n);
    return cache[n - 1];
}
