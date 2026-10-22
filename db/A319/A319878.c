/*
 * A319878  Odd numbers whose product of prime indices (A003963) is a square
 *          of a squarefree number (A062503).
 * https://oeis.org/A319878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A319878: odd numbers n for which the product of prime indices of n
   (i.e., ∏index(p)^e over n=∏p^e) is a square of a squarefree number.  
   The function a(idx_t n) returns the n-th term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*  The sieve covers indices for primes up to SIEVE_LIMIT.  It must be large
    enough to generate the required number of terms.  20 000 000 works for
    the first ten thousand terms. */
#define SIEVE_LIMIT 20000000U

static uint32_t *spf            = NULL;   /* smallest prime factor */
static uint32_t *prime_idx      = NULL;   /* 1‑based index of prime p */
static uint32_t *primes         = NULL;   /* list of primes */
static size_t    primes_cnt     = 0;      /* number of primes */

/* Static cache for the sequence values. */
static seq_t *seq_cache  = NULL;
static size_t seq_len    = 0;
static size_t seq_cap    = 0;

/* The last odd integer examined. */
static uint64_t last_checked = 1;

/* -------------------------------------------------------------------------- */
/* Build the sieve of primes, spf array, prime indices, and the primes list. */
static void build_sieve(void)
{
    if (spf != NULL)
        return;                        /* already built */

    /* Allocate arrays with explicit casts for C++ compatibility. */
    spf       = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    prime_idx = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    primes    = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    if (!spf || !prime_idx || !primes)
        exit(EXIT_FAILURE);

    /* Linear sieve: fill spf and prime indices. */
    primes_cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = i;
            prime_idx[i] = (uint32_t)(primes_cnt + 1); /* 1‑based */
            primes[primes_cnt++] = i;
        }
        uint32_t sp = spf[i];
        for (size_t j = 0; j < primes_cnt && (uint64_t)i * primes[j] <= SIEVE_LIMIT; ++j) {
            uint32_t p = primes[j];
            if (p > sp)
                break;
            spf[i * p] = p;
            if (p == sp)
                break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Ensure the cache has room for at least needed elements. */
static void cache_ensure(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        exit(EXIT_FAILURE);
    seq_cache = new_buf;
    seq_cap   = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Test whether a given odd integer n satisfies the product‑of‑indices condition. */
static bool satisfies(uint64_t n)
{
    if (n == 1)
        return true;

    /* Temporary map for the prime exponents of the product of indices. */
    /* The number of distinct primes that can appear is very small (≤ 20). */
    uint32_t map_primes[32];
    uint8_t  map_exps[32];
    size_t map_len = 0;

    uint64_t temp = n;
    while (temp > 1) {
        uint32_t p = spf[(uint32_t)temp];
        uint8_t  e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        if (p > SIEVE_LIMIT)          /* safety check, should not occur */
            return false;

        /* Factor the index of prime p. */
        uint32_t idx = prime_idx[p];   /* 1‑based index of the prime p */
        uint32_t idx_tmp = idx;
        while (idx_tmp > 1) {
            uint32_t q = spf[idx_tmp];
            uint8_t f = 0;
            while (idx_tmp % q == 0) {
                idx_tmp /= q;
                ++f;
            }
            uint32_t prod = e * f;     /* exponent to add for prime q */

            /* Insert or update q in the map. */
            size_t pos;
            for (pos = 0; pos < map_len; ++pos) {
                if (map_primes[pos] == q)
                    break;
            }
            if (pos == map_len) {
                /* new prime factor */
                if (map_len == 32)      /* guard against overflow; should not happen */
                    return false;
                map_primes[map_len] = q;
                map_exps[map_len]   = prod;
                ++map_len;
            } else {
                map_exps[pos] += prod;
            }
        }
    }

    /* All recorded exponents must be exactly 2. */
    for (size_t i = 0; i < map_len; ++i) {
        if (map_exps[i] != 2)
            return false;
    }
    return true;
}

/* -------------------------------------------------------------------------- */
/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for 0, avoid crash */

    build_sieve();

    if (seq_cache == NULL) {
        seq_cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!seq_cache)
            exit(EXIT_FAILURE);
        seq_len = 0;
        seq_cap = 16;
    }

    if (seq_len == 0) {
        seq_cache[0] = (seq_t)1;         /* the first term is 1 */
        seq_len = 1;
        last_checked = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if ((last_checked & 1ULL) == 0)   /* skip evens */
            continue;

        if (last_checked > SIEVE_LIMIT) { /* safety: our sieve ran out of room */
            return (seq_t)0;
        }

        if (satisfies(last_checked)) {
            cache_ensure(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq_cache[n - 1];
}
