/*
 * A369659  Non-multiples of 3 whose arithmetic derivative, or equally, the
 *          sum of prime factors (with multiplicity) is a multiple of 3.
 * https://oeis.org/A369659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 9581, u32 n <= 9581, u64 n <= 9581,
 *            u128 n <= 9581, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369659: Non‑multiples of 3 whose arithmetic derivative (sum of prime
   factors w/ multiplicity) is a multiple of 3.  Computed by normal
   integer sieve. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* sequence values */
static size_t cache_len = 0;         /* number of stored terms */
static size_t cache_cap = 0;         /* cached capacity */
static idx_t sieve_limit = 0;        /* current sieve upper bound */
static uint32_t *spf = NULL;         /* smallest prime factor */
static uint8_t *summod = NULL;       /* sum of prime factors mod 3 */
static uint32_t *primes = NULL;      /* list of primes */
static size_t primes_cnt = 0;
static idx_t current_num = 1;        /* next integer to test */

/* Build or rebuild the sieve up to LIMIT.  LIMIT must be >= 2. */
static void build_sieve(idx_t LIMIT)
{
    idx_t i, j;
    uint64_t prod;

    /* free old data if any */
    if (spf) free(spf);
    if (summod) free(summod);
    if (primes) free(primes);

    spf    = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    summod = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    /* rough upper bound for number of primes ~ LIMIT / 10 */
    primes = (uint32_t *)malloc((LIMIT / 10 + 100) * sizeof(uint32_t));

    if (!spf || !summod || !primes) {
        /* allocation failure; program will terminate */
        exit(1);
    }
    for (i = 0; i <= LIMIT; ++i) spf[i] = 0;
    primes_cnt = 0;

    for (i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[primes_cnt++] = (uint32_t)i;
            summod[i] = (uint8_t)(i % 3);
        }
        for (j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            prod = (uint64_t)p * i;
            if (prod > (uint64_t)LIMIT) break;
            spf[prod] = p;
            /* sum of prime factors mod 3 */
            summod[prod] = (uint8_t)((summod[i] + (p % 3)) % 3);
            if (i % p == 0) break;
        }
    }
    summod[1] = 0;          /* 1 has no prime factors, sum = 0 */
    sieve_limit = LIMIT;
}

/* Ensure the sieve covers the current number; extend if needed. */
static void ensure_sieve(idx_t need)
{
    if (need <= sieve_limit) return;
    idx_t new_limit = sieve_limit ? sieve_limit * 2 : 200000;
    while (new_limit < need) new_limit *= 2;
    build_sieve(new_limit);
}

/* Ensure the cache holds at least N terms. */
static void ensure_cache(idx_t N)
{
    /* Grow cache if necessary */
    while (cache_len < (size_t)N) {
        /* extend cache capacity if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* ensure sieve large enough for the next number */
        ensure_sieve(current_num + 1);

        if (current_num % 3 != 0 && summod[(idx_t)current_num] == 0) {
            cache[cache_len++] = (seq_t)current_num;
        }
        current_num++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* out of range */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
