/*
 * A318527  Numbers k such that -3 is a quadratic residue (not necessarily
 *          coprime) modulo k, k + 1, k + 2 and k + 3.
 * https://oeis.org/A318527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A318527: numbers k such that -3 is a quadratic residue
   modulo k, k+1, k+2 and k+3.  The terms are 1 mod 360.
   The implementation searches forward over k ≡ 1 (mod 360) and
   uses prime factorisation to test the quadratic‑residue condition. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve --------------------------------------------------- */
static uint32_t *prime_list = NULL;   /* array of primes, 2 <= p <= PRIMELIMIT */
static size_t     prime_cnt = 0;       /* number of primes in prime_list */
#define PRIMELIMIT 1000000            /* enough for k up to ~10^12 */

static void generate_primes(void)
{
    if (prime_list != NULL) return;           /* already generated */

    /* Allocate boolean sieve array */
    bool *sieve = (bool*)calloc(PRIMELIMIT + 1, sizeof(bool));
    if (!sieve) abort();

    /* Simple Eratosthenes sieve */
    for (uint32_t i = 2; i * i <= PRIMELIMIT; ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= PRIMELIMIT; j += i)
                sieve[j] = true;

    /* Count primes */
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i)
        if (!sieve[i]) ++prime_cnt;

    /* Allocate array for primes */
    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = i;

    free(sieve);
}

/* ---------- Test whether -3 is a quadratic residue modulo m ------------- */
static bool minus3_residue(uint64_t m)
{
    if (m == 1) return true;            /* trivial residue 0 */

    uint64_t n = m;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        if (n % p != 0) continue;

        unsigned exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }

        if (p == 2) {
            if (exp > 2) return false; /* 2^e for e>2 fails */
        } else if (p == 3) {
            if (exp > 1) return false; /* 3^e for e>1 fails */
        } else {                       /* odd prime > 3 */
            if (p % 6 != 1) return false; /* require p ≡ 1 (mod 6) */
        }
    }

    if (n > 1) {        /* remaining prime factor > sqrt(original m) */
        uint32_t p = (uint32_t)n;
        if (p == 2) return true;       /* exponent 1 is ok */
        if (p == 3) return true;       /* exponent 1 is ok */
        if (p % 6 != 1) return false;
    }

    return true;
}

/* ---------- Cache for computed terms ------------------------------------ */
static seq_t *cache = NULL;           /* dynamic array of terms */
static size_t cache_size = 0;          /* number of cached terms */
static size_t cache_cap   = 0;          /* current capacity of cache */
static uint64_t last_k = 0;            /* last candidate k examined */

/* Append a value to the cache, resizing if necessary */
static void cache_push(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* ---------- Main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside the offset */

    /* Initialise on first call */
    if (cache_size == 0) {
        generate_primes();            /* primes up to PRIMELIMIT */
        cache_push((seq_t)1);          /* a(1) = 1 */
        last_k = 1;                    /* last candidate consumed */
    }

    /* Generate terms until we have at least n terms cached */
    while (cache_size < (size_t)n) {
        /* Next candidate k ≡ 1 (mod 360) */
        last_k += 360;

        bool good = true;
        for (uint32_t off = 0; off < 4; ++off) {
            if (!minus3_residue(last_k + off)) {
                good = false;
                break;
            }
        }

        if (good)
            cache_push((seq_t)last_k);
    }

    return cache[n - 1];
}
