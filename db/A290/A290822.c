/*
 * A290822  Transitive numbers: Matula-Goebel numbers of transitive rooted
 *          trees.
 * https://oeis.org/A290822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 2684, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   contains_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Transitive numbers (OEIS A290822)
 * This routine generates the sequence of positive integers x
 * such that for every prime p dividing x, every prime dividing the
 * index of p among the primes also divides x.
 * The sequence is produced on demand, cached, and returned for
 * any index n ≥ 1.  The same algorithm works for 8‑, 16‑, 32‑,
 * 64‑bit and arbitrary‑precision builds.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains_prime.h"

/* --------------------------------------------------------------- */
/* Small static prime sieve – enough for the range needed by the
 * test (≈3·10⁶ primes).  This is safe for n ≤ 10000 and gives
 * all prime factors of the numbers occurring in the sequence
 * within the tested range (see the OEIS file for the growth).   */
#define PRIME_LIMIT 3000000ULL   /* upper bound for sieve */

static uint32_t *PRIMES = NULL;          /* list of primes */
static size_t    PRIMES_COUNT = 0;        /* number of primes */
static uint32_t *PRIME_INDEX = NULL;     /* index of a prime, 0 if composite */
static bool      PRIMES_INITIALIZED = false;

static void init_primes(void)
{
    if (PRIMES_INITIALIZED) return;
    PRIMES_INITIALIZED = true;

    /* simple sieve of Eratosthenes */
    uint8_t *is_comp = (uint8_t *)malloc(PRIME_LIMIT + 1);
    for (uint64_t i = 0; i <= PRIME_LIMIT; ++i) is_comp[i] = 0;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;

    /* collect primes */
    PRIMES = (uint32_t *)malloc(300000 * sizeof(uint32_t));
    PRIMES_COUNT = 0;
    PRIME_INDEX = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    for (uint64_t i = 0; i <= PRIME_LIMIT; ++i) PRIME_INDEX[i] = 0;

    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) {
            if (PRIMES_COUNT == 300000) {   /* very unlikely, but safe */
                PRIMES = (uint32_t *)realloc(PRIMES,
                    (PRIMES_COUNT + 300000) * sizeof(uint32_t));
            }
            PRIMES[PRIMES_COUNT] = (uint32_t)i;
            ++PRIMES_COUNT;
            PRIME_INDEX[i] = (uint32_t)PRIMES_COUNT; /* 1‑based index */
        }

    free(is_comp);
}

static size_t prime_index(uint64_t p)
{
    init_primes();
    if (p <= PRIME_LIMIT)
        return (size_t)PRIME_INDEX[p];
    /* binary search on PRIMES array */
    size_t lo = 0, hi = PRIMES_COUNT;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t val = PRIMES[mid];
        if (val == (uint32_t)p) return mid + 1;
        if (val < (uint32_t)p) lo = mid + 1;
        else hi = mid;
    }
    return 0; /* not a prime */
}

/* --------------------------------------------------------------- */
/* Transitivity test for a 64‑bit integer;  the routine never
 * exceeds 64‑bit arithmetic because all intermediate values are
 * at most the candidate number itself. */
static bool is_transitive(uint64_t x)
{
    if (x == 1) return true;

    /* distinct prime factors of x */
    uint64_t pf[64];
    size_t pf_cnt = 0;
    uint64_t tmp = x;

    for (size_t i = 0; i < PRIMES_COUNT &&
                     (uint64_t)PRIMES[i] * (uint64_t)PRIMES[i] <= tmp;
         ++i) {
        uint32_t p = PRIMES[i];
        if (tmp % p == 0) {
            pf[pf_cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) pf[pf_cnt++] = tmp;   /* remaining prime factor */

    /* For every p | x, ensure every prime dividing index(p) also divides x */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf[i];
        size_t idx = prime_index(p);
        if (idx <= 1) continue; /* index 1 has no prime divisors */

        /* factorise idx */
        uint64_t t = idx;
        for (size_t k = 0; k < PRIMES_COUNT &&
                          (uint64_t)PRIMES[k] * (uint64_t)PRIMES[k] <= t;
             ++k) {
            uint32_t q = PRIMES[k];
            if (t % q == 0) {
                if (!contains_prime((uint64_t)q, pf, pf_cnt))
                    return false;
                while (t % q == 0) t /= q;
            }
        }
        if (t > 1 && !contains_prime(t, pf, pf_cnt))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/* Cache for transitive numbers (as seq_t).  The cache is built
 * lazily and stored in natural integer form (uint64_t), then
 * converted to seq_t when inserted. */
static seq_t *CACHE = NULL;
static size_t CACHE_SIZE = 0;
static size_t CACHE_CAP  = 0;
static uint64_t CACHE_LAST = 0;   /* highest integer stored in the cache */

static void cache_append(uint64_t val)
{
    if (CACHE_SIZE == CACHE_CAP) {
        size_t new_cap = (CACHE_CAP == 0) ? 16 : CACHE_CAP * 2;
        CACHE = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
        CACHE_CAP = new_cap;
    }
    CACHE[CACHE_SIZE++] = (seq_t)val;
    CACHE_LAST = val;
}

/* --------------------------------------------------------------- */
/* Public sequence routine required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, return 0 */

    size_t target = (size_t)n;              /* 1‑based index */

    init_primes();                          /* ensure primes ready */

    /* extend cache until it contains target elements */
    for (uint64_t cand = CACHE_LAST + 1;
         CACHE_SIZE < target;
         ++cand) {
        if (is_transitive(cand))
            cache_append(cand);
    }

    return CACHE[target - 1];
}
