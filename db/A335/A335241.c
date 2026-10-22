/*
 * A335241  Numbers whose prime indices are not pairwise coprime, where a
 *          singleton is not coprime unless it is {1}.
 * https://oeis.org/A335241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335241 – Numbers whose prime indices are not pairwise coprime.
   Implementation uses a sieve of Eratosthenes up to a fixed limit (2 000 000)
   to obtain prime indices, then iterates natural numbers, testing the
   coprimality condition by factorisation into these primes.  Results are
   cached so that arbitrary call order is handled efficiently.  The code
   compiles in C11 and C++17.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime table (static for the whole program) ---------- */
static uint32_t *primes = NULL;        /* primes[i]   (0‑based)           */
static uint32_t *prime_index = NULL;  /* prime_index[p] = 1‑based index of prime p (0 if p not prime) */
static size_t   prime_count = 0;
static bool     primes_built = false;

/* Build primes up to LIMIT = 2 000 000.  This bound is sufficient for
   n up to about 10 000.  If a larger bound is needed the program can
   be re‑compiled with a higher LIMIT. */
static void build_primes(void)
{
    if (primes_built) return;
    primes_built = true;

    const size_t LIMIT = 2000000;          /* adjust if necessary          */
    bool *is_prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes and store them.  */
    size_t count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++count;

    primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    prime_index = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!prime_index) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[idx]       = (uint32_t)i;
            prime_index[i]    = (uint32_t)(idx + 1); /* 1‑based index */
            ++idx;
        }
    }
    prime_count = count;
    free(is_prime);
}

/* ---------- Euclidean gcd for 32‑bit integers ---------- */
static uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- Test whether n qualifies for the sequence ---------- */
static bool qualifies(uint64_t n)
{
    if (n == 1) return true;                 /* empty set is in the sequence */

    uint64_t tmp = n;
    uint32_t indices[32];
    uint32_t exps[32];
    size_t   cnt = 0;

    /* Factorisation by trial division using the pre‑computed primes. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            uint32_t c = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++c;
            }
            indices[cnt] = prime_index[p];
            exps[cnt]    = c;
            ++cnt;
        }
    }
    if (tmp > 1) {
        /* Remaining factor is prime.  It is guaranteed to be <= LIMIT. */
        indices[cnt] = prime_index[(uint32_t)tmp];
        exps[cnt]    = 1;
        ++cnt;
    }

    if (cnt == 1) {
        /* Singleton.  Include iff the index is not 1 (i.e., prime ≠ 2). */
        return indices[0] != 1;
    }

    /* Multiple distinct prime factors.  Check exponents > 1 for index>1. */
    for (size_t i = 0; i < cnt; ++i) {
        if (indices[i] > 1 && exps[i] > 1) return true;
    }

    /* Check pairwise gcd of distinct indices. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (gcd32(indices[i], indices[j]) > 1) return true;
        }
    }
    return false;
}

/* ---------- Sequence cache (static, thread‑unsafe; suitable for single
           thread or shared constant access) ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;      /* next integer to test */

static void cache_init(void)
{
    if (cache_len > 0) return;             /* already initialised */
    build_primes();                         /* ensure prime tables */

    /* Allocate initial cache and store the first term (1). */
    cache_cap   = 16;
    cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache[0] = (seq_t)1;
    cache_len = 1;
}

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) exit(EXIT_FAILURE);
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- Main function required by the build system ---------- */
seq_t a(idx_t n)
{
    cache_init();
    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate;
        while (!qualifies(cand)) ++cand;
        cache_append((seq_t)cand);
        next_candidate = cand + 1;
    }
    return cache[n - 1];
}
