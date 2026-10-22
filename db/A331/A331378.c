/*
 * A331378  Numbers whose product of prime indices is divisible by their sum
 *          of prime factors.
 * https://oeis.org/A331378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that (∏_{p^e‖k} (prime_index(p))^e)
 *            mod (∑_{p^e‖k} e·p) == 0, where prime_index(p) is the 1‑based
 *            index of prime p in the increasing list of primes (2 has index
 *            1)
 * Range:     u8 n <= 6, u16 n <= 746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

 /* ------------------------------------------------------------------ */
 /*  Global data (initialised on first call)                            */
 /* ------------------------------------------------------------------ */
#define MAX_LIMIT 10000000ULL          /* upper bound for SPF table */
static uint32_t *spf = NULL;          /* spf[i] = smallest prime factor of i */
static uint32_t *prime_index = NULL;  /* prime_index[p] = 1‑based index of prime p */
static size_t    prime_count = 0;     /* number of primes <= MAX_LIMIT */

static seq_t *cache = NULL;           /* memoised values of a(n) */
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* ------------------------------------------------------------------ */
static void init_sieve(void)
{
    if (spf != NULL) return;          /* already initialised */

    /* Allocate tables */
    spf            = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    prime_index    = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!spf || !prime_index) abort();

    memset(spf, 0, (MAX_LIMIT + 1) * sizeof(uint32_t));
    memset(prime_index, 0, (MAX_LIMIT + 1) * sizeof(uint32_t));

    /* Linear sieve for SPF and prime list */
    uint32_t *primes = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!primes) abort();
    size_t pc = 0;

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            primes[pc++] = i;
            if ((uint64_t)i * (uint64_t)i <= MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    prime_count = pc;

    /* Build prime index table */
    for (size_t idx = 0; idx < prime_count; ++idx)
        prime_index[primes[idx]] = (uint32_t)(idx + 1);

    free(primes);
}

/* ------------------------------------------------------------------ */
static bool satisfies(uint64_t x)
{
    if (x < 2) return false;

    uint32_t primes_arr[8];
    uint32_t exps_arr[8];
    size_t   cnt = 0;

    uint64_t tmp = x;

    /* Factorisation using SPF (with multiplicity) */
    while (tmp > 1) {
        uint32_t p = spf[(uint32_t)tmp];
        uint32_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        primes_arr[cnt] = p;
        exps_arr[cnt]   = e;
        cnt++;
    }

    /* Compute sum of prime factors (with multiplicity) */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)exps_arr[i] * (seq_t)primes_arr[i];

    if (sum == 0) return false;  /* should not happen */

    /* Compute product of prime indices (with multiplicity) modulo sum */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t idx = (seq_t)prime_index[primes_arr[i]];
        for (uint32_t j = 0; j < exps_arr[i]; ++j)
            prod = (prod * idx) % sum;
    }

    return prod == 0;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offsets < 1 */

    init_sieve();

    /* Ensure cache has at least n entries */
    while (cache_size < (size_t)n) {
        if (satisfies(next_candidate)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
